#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
#ifdef __EMSCRIPTEN__
#include <SDL_mixer.h>
#include <emscripten.h>
#include <filesystem>
#include <format>
#include <gsl/pointers>
#include <iterator>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/services.h"

#include "file_loaders.h"
#include "utils.h"

// clang-format off
// cppcheck-suppress-begin internalAstError
EM_ASYNC_JS(char*, em_read_file, (const char* file_path_cstr, const char* parent_path_cstr), {
  const file_path = UTF8ToString(file_path_cstr);
  const parent_path = UTF8ToString(parent_path_cstr);
  const parent_info = FS.analyzePath(parent_path, true);
  async function sync() {
    return new Promise((resolve, reject) => {
      FS.syncfs(true, (err) => {
        if (err) {
          console.error("Can't synchronize IDBFS store");
          reject();
        } else {
          resolve();
        }
      });
    });
  }
  if (!parent_info.exists) {
    FS.mkdir(parent_path);
  }
  FS.mount(IDBFS, {}, parent_path);
  return await sync()
    .then(() => FS.readFile(file_path, { encoding: 'utf8' }))
    .catch(() => {
      console.warn(`File not readable: ${file_path}`);
      return "{}";
    })
    .then((content) => stringToNewUTF8(content))
    .finally(() => FS.unmount(parent_path));
});
// cppcheck-suppress-end internalAstError
// clang-format on

std::string
load_string_by_emscripten(const std::string& file_name)
{
  gsl::owner<char*> loaded_data = nullptr;
  int loaded_size = 0;
  int error = 0;
  emscripten_wget_data(file_name.c_str(),
                       reinterpret_cast<void**>(&loaded_data), // NOLINT
                       &loaded_size,
                       &error);
  if (error > 0) {
    delete loaded_data;
    throw std::runtime_error(
      std::format("Can't load a text file: {}", file_name));
  }
  std::string data(loaded_data, loaded_size);
  delete loaded_data;
  return data;
}

std::vector<unsigned char>
load_binary_by_emscripten(const std::string& file_name)
{
  gsl::owner<unsigned char*> loaded_data = nullptr;
  int loaded_size = 0;
  int error = 0;
  emscripten_wget_data(file_name.c_str(),
                       reinterpret_cast<void**>(&loaded_data), // NOLINT
                       &loaded_size,
                       &error);
  if (error > 0) {
    delete loaded_data;
    throw std::runtime_error(
      std::format("Can't load a binary file: {}", file_name));
  }
  std::vector<unsigned char> data(loaded_data, loaded_data + loaded_size);
  delete loaded_data;
  return data;
}

tinygltf::Model
load_gltf_model(const std::string& file_name)
{
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;

  auto data = load_string_by_emscripten(file_name);
  bool status = loader.LoadBinaryFromMemory(
    &model,
    &err,
    &warn,
    reinterpret_cast<const unsigned char*>(data.c_str()), // NOLINT
    data.size(),
    file_name);

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
  return load_string_by_emscripten(file_name);
}

std::vector<unsigned char>
load_binary(const std::string& file_name)
{
  return load_binary_by_emscripten(file_name);
}

nlohmann::basic_json<>
load_json(const std::string& file_path)
{
  auto data = load_string_by_emscripten(file_path);
  return nlohmann::json::parse(begin(data), end(data));
}

nlohmann::basic_json<>
load_local_json(const std::filesystem::path& file_path)
{
  auto parent_path = file_path.parent_path();
  auto raw_data = std::make_unique<char*>(
    em_read_file(file_path.c_str(), parent_path.c_str()));
  auto json = nlohmann::json::parse(*raw_data);
  return json;
}

void
save_local_json(const std::filesystem::path& file_path, nlohmann::json json)
{
  auto dump = json.dump();
  auto parent_path = file_path.parent_path();
  // clang-format off
  EM_ASM({
    const file_path = UTF8ToString($0);
    const parent_path = UTF8ToString($1);
    const data = UTF8ToString($2);
    const parent_info = FS.analyzePath(parent_path, true);
    if (!parent_info.exists) {
      FS.mkdir(parent_path);
    }
    FS.mount(IDBFS, {}, parent_path);
    FS.syncfs(true, (err) => {
      if (err) {
        console.error(err);
      } else {
        FS.writeFile(file_path, data);
        FS.syncfs((err) => {
          if (err) {
            console.error(err);
          } else {
            FS.unmount(parent_path);
          }
        });
      }
    });
  }, file_path.c_str(), parent_path.c_str(), dump.c_str());
  // clang-format on
}

#endif
