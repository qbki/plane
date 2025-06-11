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
#include "src/utils/result.h" // IWYU pragma: export

#include "file_loaders.h"
#include "utils.h"

// clang-format off
// cppcheck-suppress-begin internalAstError
EM_JS(void, register_psync, (), {
  if (!FS.psync) {
    FS["psync"] = (is_loading = true) => (
      new Promise((resolve, reject) => {
        FS.syncfs(is_loading, (err) => {
          if (err) {
            console.error("Can't synchronize IDBFS store");
            reject();
          } else {
            resolve();
          }
        });
      })
    );
  }
});
// cppcheck-suppress-end internalAstError
// clang-format on

// clang-format off
// cppcheck-suppress-begin internalAstError
EM_ASYNC_JS(char*, em_read_file, (const char* file_path_cstr, const char* parent_path_cstr), {
  register_psync();
  const file_path = UTF8ToString(file_path_cstr);
  const parent_path = UTF8ToString(parent_path_cstr);
  const parent_info = FS.analyzePath(parent_path, true);
  if (!parent_info.exists) {
    FS.mkdir(parent_path);
  }
  FS.mount(IDBFS, {}, parent_path);
  return await FS.psync()
    .then(() => FS.readFile(file_path, { encoding: 'utf8' }))
    .catch(() => {
      console.warn(`File is not readable: ${file_path}`);
      return "{}";
    })
    .then((content) => stringToNewUTF8(content))
    .finally(() => FS.unmount(parent_path));
});
// cppcheck-suppress-end internalAstError
// clang-format on

// clang-format off
// cppcheck-suppress-begin internalAstError
EM_ASYNC_JS(void, em_store_file, (const char* file_path_cstr, const char* parent_path_cstr, const char* data_cstr),
{
  register_psync();
  const file_path = UTF8ToString(file_path_cstr);
  const parent_path = UTF8ToString(parent_path_cstr);
  const data = UTF8ToString(data_cstr);
  const parent_info = FS.analyzePath(parent_path, true);
  if (!parent_info.exists) {
    FS.mkdir(parent_path);
  }
  FS.mount(IDBFS, {}, parent_path);
  await FS.psync()
    .then(() => FS.writeFile(file_path, data))
    .then(() => FS.psync(false))
    .catch(() => console.warn(`File is not readable: ${file_path}`))
    .finally(() => FS.unmount(parent_path));
});
// cppcheck-suppress-end internalAstError
// clang-format on

Result<std::string>
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
    auto message = std::format("Can't load a text file: {}", file_name);
    return Result<std::string>::from_error(
      std::make_shared<std::runtime_error>(message));
  }
  std::string data(loaded_data, loaded_size);
  delete loaded_data;
  return Result<std::string>::from_payload(data);
}

Result<std::vector<char>>
load_binary_by_emscripten(const std::string& file_name)
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
    auto message = std::format("Can't load a binary file: {}", file_name);
    return Result<std::vector<char>>::from_error(
      std::make_shared<std::runtime_error>(message));
  }
  std::vector<char> data(loaded_data, loaded_data + loaded_size);
  delete loaded_data;
  return Result<std::vector<char>>::from_payload(data);
}

Result<tinygltf::Model>
load_gltf_model(const std::string& file_name)
{
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err;
  std::string warn;

  auto data_result = load_string_by_emscripten(file_name);
  if (!data_result.has_payload()) {
    return Result<tinygltf::Model>::from_error(data_result.error());
  }
  auto data = data_result.or_crash();
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

  if (status) {
    Services::logger().info(std::format("Loaded glTF: {}", file_name));
    return Result<tinygltf::Model>::from_payload(model);
  }
  auto message = std::format("Failed to load glTF: {}", file_name);
  return Result<tinygltf::Model>::from_error(
    std::make_shared<std::runtime_error>(message));
}

Result<std::string>
load_text(const std::string& file_name)
{
  return load_string_by_emscripten(file_name);
}

Result<std::vector<char>>
load_binary(const std::string& file_name)
{
  return load_binary_by_emscripten(file_name);
}

Result<nlohmann::basic_json<>>
load_json(const std::string& file_path)
{
  return load_string_by_emscripten(file_path).then<nlohmann::basic_json<>>(
    [](const std::string& data) {
      return Result<nlohmann::basic_json<>>::from_payload(
        nlohmann::json::parse(begin(data), end(data)));
    });
}

Result<nlohmann::basic_json<>>
load_local_json(const std::filesystem::path& file_path)
{
  auto parent_path = file_path.parent_path();
  auto raw_data = std::make_unique<char*>(
    em_read_file(file_path.c_str(), parent_path.c_str()));
  auto json = nlohmann::json::parse(*raw_data);
  return Result<nlohmann::basic_json<>>::from_payload(json);
}

void
save_local_json(const std::filesystem::path& file_path, nlohmann::json json)
{
  auto dump = json.dump();
  auto parent_path = file_path.parent_path();
  em_store_file(file_path.c_str(), parent_path.c_str(), dump.c_str());
}

bool
is_file_exists(const std::filesystem::path&)
{
  crash(std::format("{} can't be used in WASM build", __func__));
}

#endif
