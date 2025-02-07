#include <SDL_mixer.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/mesh.h"
#include "src/services.h"
#include "src/utils/result.h" // IWYU pragma: export

#include "file_loaders.h"
#include "utils.h"

using FileStreamPtr = std::shared_ptr<std::fstream>;

Result<FileStreamPtr>
open_file_stream(const std::string& path, std::ios::openmode mode)
{
  auto stream = std::make_shared<std::fstream>(path, mode);
  static auto make_error = [&path]() {
    auto message = std::format("File \"{}\" is not readable", path);
    return std::make_shared<std::runtime_error>(message);
  };
  return stream->fail()
           ? Result<std::shared_ptr<std::fstream>>::from_error(make_error())
           : Result<std::shared_ptr<std::fstream>>::from_payload(
               std::move(stream));
}

Result<FileStreamPtr>
open_text_file_stream(const std::string& path)
{
  return open_file_stream(path, std::ios_base::in | std::ios_base::out);
}

Result<FileStreamPtr>
open_binary_file_stream(const std::string& path)
{
  return open_file_stream(
    path, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
}

Result<tinygltf::Model>
load_gltf_model(const std::string& file_path)
{
  return does_file_exist(file_path).then<tinygltf::Model>(
    [](const std::string& path) {
      std::string err;
      std::string warn;
      tinygltf::Model model;
      tinygltf::TinyGLTF loader;
      bool status = loader.LoadBinaryFromFile(&model, &err, &warn, path);

      if (!warn.empty()) {
        Services::logger().warn(warn);
      }
      if (!err.empty()) {
        Services::logger().error(err);
      }

      if (status) {
        Services::logger().info(std::format("Loaded glTF: {}", path));
        return Result<tinygltf::Model>::from_payload(std::move(model));
      }
      auto error = std::make_shared<std::runtime_error>(
        std::format("Failed to load glTF: {}", path));
      return Result<tinygltf::Model>::from_error(error);
    });
}

Result<std::string>
load_text(const std::string& file_path)
{
  auto read_file = [](const FileStreamPtr& stream) {
    std::string data { std::istreambuf_iterator<char>(*stream),
                       std::istreambuf_iterator<char>() };
    return Result<std::string>::from_payload(std::move(data));
  };
  return does_file_exist(file_path)
    .then<FileStreamPtr>(open_text_file_stream)
    .then<std::string>(read_file);
}

Result<std::vector<unsigned char>>
load_binary(const std::string& file_path)
{
  auto read_file = [](FileStreamPtr& stream) {
    std::vector<unsigned char> data { std::istreambuf_iterator<char>(*stream),
                                      std::istreambuf_iterator<char>() };
    return Result<std::vector<unsigned char>>::from_payload(std::move(data));
  };
  return Result<const std::string>::from_payload(file_path)
    .then<const std::string>(does_file_exist)
    .then<FileStreamPtr>(open_binary_file_stream)
    .then<std::vector<unsigned char>>(read_file);
}

Result<nlohmann::basic_json<>>
load_json(const std::string& file_path)
{
  auto parse = [&file_path](FileStreamPtr& stream) {
    Services::logger().info(std::format("Loaded json: {}", file_path));
    return Result<nlohmann::basic_json<>>::from_payload(
      nlohmann::json::parse(*stream));
  };
  return does_file_exist(file_path)
    .then<FileStreamPtr>(open_text_file_stream)
    .then<nlohmann::basic_json<>>(parse);
}

Result<nlohmann::basic_json<>>
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

bool
is_file_exists(const std::filesystem::path& path)
{
  return std::filesystem::exists(path);
}
