#include <SDL_mixer.h>
#include <filesystem>
#include <format>
#include <memory>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

#include "src/services.h"

#include "file_loaders.h"
#ifdef __EMSCRIPTEN__
#include "file_loaders.ems.ipp"
#else
#include "file_loaders.std.ipp"
#endif

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
