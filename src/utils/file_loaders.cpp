#include <SDL_mixer.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <iterator>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <sstream>
#endif

#include "src/services.h"

#include "file_loaders.h"

void
throw_error_if_file_not_found(const std::string& file_name)
{
  if (!std::filesystem::exists(file_name)) {
    throw std::runtime_error(std::format("File not found: {}", file_name));
  }
}

void
throw_error_if_file_is_not_readable(const std::istream& stream,
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
  if (error > 0) {
    delete loaded_data;
    throw std::runtime_error(
      std::format("Can't load a text file: {}", file_name));
  }
  std::string data(loaded_data, loaded_data + loaded_size);
  delete loaded_data;
  return data;
}

std::vector<unsigned char>
load_binary_by_emscripten(const std::string& file_name)
{
  unsigned char* loaded_data;
  int loaded_size = 0;
  int error = 0;
  emscripten_wget_data(file_name.c_str(),
                       reinterpret_cast<void**>(&loaded_data),
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
#ifdef __EMSCRIPTEN__
  return load_string_by_emscripten(file_name);
#else
  throw_error_if_file_not_found(file_name);
  std::fstream file_stream { file_name };
  throw_error_if_file_is_not_readable(file_stream, file_name);
  return { std::istreambuf_iterator<char>(file_stream),
           std::istreambuf_iterator<char>() };
#endif
}

std::vector<unsigned char>
load_binary(const std::string& file_name)
{
#ifdef __EMSCRIPTEN__
  return load_binary_by_emscripten(file_name);
#else
  throw_error_if_file_not_found(file_name);
  std::ifstream file_stream { file_name, std::ios::binary };
  throw_error_if_file_is_not_readable(file_stream, file_name);
  return { std::istreambuf_iterator<char>(file_stream),
           std::istreambuf_iterator<char>() };
#endif
}

nlohmann::basic_json<>
load_json(const std::string& file_path)
{
#ifdef __EMSCRIPTEN__
  auto data = load_string_by_emscripten(file_path);
  return nlohmann::json::parse(begin(data), end(data));
#else
  throw_error_if_file_not_found(file_path);
  auto file_stream = std::fstream(file_path);
  throw_error_if_file_is_not_readable(file_stream, file_path);
  return nlohmann::json::parse(file_stream);
#endif
}

#ifdef __EMSCRIPTEN__
// cppcheck-suppress-begin internalAstError
EM_ASYNC_JS(char*, em_read_file, (const char* file_path_cstr, const char* parent_path_cstr), {
  const file_path = UTF8ToString(file_path_cstr);
  const parent_path = UTF8ToString(parent_path_cstr);
  const parent_info = FS.analyzePath(parent_path, true);
  async function sync() {
    return new Promise((resolve, reject) => {
      FS.syncfs(true, (err) => {
        if (err) {
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
    .then((content) => stringToNewUTF8(content))
    .finally(() => FS.unmount(parent_path));
});
// cppcheck-suppress-end internalAstError
#endif

nlohmann::basic_json<>
load_local_json(const std::filesystem::path& file_path)
{
#ifdef __EMSCRIPTEN__
  auto parent_path = file_path.parent_path();
  auto settings_raw_data = em_read_file(file_path.c_str(), parent_path.c_str());
  auto json = nlohmann::json::parse(settings_raw_data);
  free(settings_raw_data);
  return json;
#else
  return load_json(file_path);
#endif
}

void
save_local_json(const std::filesystem::path& file_path, nlohmann::json json)
{
  auto dump = json.dump();
  auto parent_path = file_path.parent_path();
#ifdef __EMSCRIPTEN__
  EM_ASM({
    const file_path = UTF8ToString($0);
    const parent_path = UTF8ToString($1);
    const data = UTF8ToString($2);
    const parent_info = FS.analyzePath(parent_path, true);
    if (!parent_info.exists) {
      FS.mkdir(parent_path);
    }
    FS.mount(IDBFS, {}, parent_path);
    FS.syncfs((err) => {
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
#else
  if (!std::filesystem::exists(parent_path)
      && !std::filesystem::create_directories(parent_path))
  {
    auto message = std::format("Can't create a directory: {}", parent_path.string());
    Services::logger().error(message);
    return;
  }
  std::ofstream file {file_path};
  if (file) {
    file << dump;
  }
#endif
}

std::shared_ptr<DataHolder>
load_sdl_rw_data(const std::filesystem::path& path)
{
  auto raw_data = load_binary(path);
  Services::logger().info(
    std::format("Loaded: {} ({})", path.c_str(), raw_data.size()));
  return std::make_shared<DataHolder>(std::move(raw_data));
}

std::unique_ptr<Sound::Sound>
load_sound(const std::filesystem::path& sound_file_path)
{
  auto holder = load_sdl_rw_data(sound_file_path);
  auto rwops = holder->rwops();
  auto chunk = Mix_LoadWAV_RW(rwops.get(), 0);
  return std::make_unique<Sound::Sound>(chunk);
}
