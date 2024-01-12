#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <iostream>
#include <sstream>
#endif

#include "../services.h"
#include "file_loaders.h"

void
throw_error_if_file_not_found(const std::string& file_name)
{
  if (!std::filesystem::exists(file_name)) {
    throw std::runtime_error(std::format("File not found: {}", file_name));
  }
}

void
throw_error_if_file_is_not_readable(const std::fstream& stream,
                                    const std::string& file_name)
{
  if (stream.fail()) {
    throw std::runtime_error(std::format("Cannot read a file: {}", file_name));
  }
}

#ifdef __EMSCRIPTEN__
std::string
load_string_by_emscripten(const std::string& file_name)
{
  char* loaded_data;
  int loaded_size = 0;
  int error = 0;
  emscripten_wget_data(file_name.c_str(),
                       reinterpret_cast<void**>(&loaded_data),
                       &loaded_size,
                       &error);
  std::string data(loaded_data, loaded_data + loaded_size);
  delete loaded_data;
  return data;
}
#endif

tinygltf::Model
load_gltf_model(const std::string& file_name)
{
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;

#ifdef __EMSCRIPTEN__
  auto data = load_string_by_emscripten(file_name);
  bool status = loader.LoadBinaryFromMemory(
    &model,
    &err,
    &warn,
    reinterpret_cast<const unsigned char*>(data.c_str()),
    data.size(),
    file_name);
#else
  throw_error_if_file_not_found(file_name);
  bool status = loader.LoadBinaryFromFile(&model, &err, &warn, file_name);
#endif

  if (!warn.empty()) {
    logger().warn(warn);
  }

  if (!err.empty()) {
    logger().error(err);
  }

  if (!status) {
    throw std::runtime_error(std::format("Failed to load glTF: {}", file_name));
  } else {
    logger().info(std::format("Loaded glTF: {}", file_name));
  }

  return model;
}

std::string
load_text(const std::string& file_name)
{
  throw_error_if_file_not_found(file_name);
  std::fstream file_stream{ file_name };
  throw_error_if_file_is_not_readable(file_stream, file_name);
  return { std::istreambuf_iterator<char>(file_stream),
           std::istreambuf_iterator<char>() };
}

nlohmann::basic_json<>
load_json(const std::string& file_name)
{
#ifdef __EMSCRIPTEN__
  auto data = load_string_by_emscripten(file_name);
  return nlohmann::json::parse(begin(data), end(data));
#else
  throw_error_if_file_not_found(file_name);
  auto file_stream = std::fstream(file_name);
  throw_error_if_file_is_not_readable(file_stream, file_name);
  return nlohmann::json::parse(file_stream);
#endif
}
