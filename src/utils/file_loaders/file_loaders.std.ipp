#include <SDL_mixer.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/services.h"

#include "file_loaders.h"
#include "utils.h"

tinygltf::Model
load_gltf_model(const std::string& file_name)
{
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;

  throw_error_if_file_not_found(file_name);
  bool status = loader.LoadBinaryFromFile(&model, &err, &warn, file_name);

  if (!warn.empty()) {
    Services::logger().warn(warn);
  }

  if (!err.empty()) {
    Services::logger().error(err);
  }

  if (!status) {
    throw std::runtime_error(std::format("Failed to load glTF: {}", file_name));
  } else {
    Services::logger().info(std::format("Loaded glTF: {}", file_name));
  }

  return model;
}

std::string
load_text(const std::string& file_name)
{
  throw_error_if_file_not_found(file_name);
  std::fstream file_stream { file_name };
  throw_error_if_file_is_not_readable(file_stream, file_name);
  return { std::istreambuf_iterator<char>(file_stream),
           std::istreambuf_iterator<char>() };
}

std::vector<unsigned char>
load_binary(const std::string& file_name)
{
  throw_error_if_file_not_found(file_name);
  std::ifstream file_stream { file_name, std::ios::binary };
  throw_error_if_file_is_not_readable(file_stream, file_name);
  return { std::istreambuf_iterator<char>(file_stream),
           std::istreambuf_iterator<char>() };
}

nlohmann::basic_json<>
load_json(const std::string& file_path)
{
  throw_error_if_file_not_found(file_path);
  auto file_stream = std::fstream(file_path);
  throw_error_if_file_is_not_readable(file_stream, file_path);
  return nlohmann::json::parse(file_stream);
}

nlohmann::basic_json<>
load_local_json(const std::filesystem::path& file_path)
{
  return load_json(file_path);
}

void
save_local_json(const std::filesystem::path& file_path, nlohmann::json json)
{
  auto dump = json.dump();
  auto parent_path = file_path.parent_path();
  if (!std::filesystem::exists(parent_path)
      && !std::filesystem::create_directories(parent_path)) {
    auto message = std::format("Can't create a directory: {}",
                               parent_path.string());
    Services::logger().error(message);
    return;
  }
  std::ofstream file { file_path };
  if (file) {
    file << dump;
  }
}
