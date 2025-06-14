#include <SDL_mixer.h>
#include <filesystem>
#include <format>
#include <memory>
#include <thorvg.h>
#include <utility>
#include <vector>

#include "src/utils/result.h"
#include "src/utils/tvg.h"

#include "file_loaders.h"
#ifdef __EMSCRIPTEN__
#include "file_loaders.ems.ipp"
#else
#include "file_loaders.std.ipp"
#endif

import pln.services.logger;

std::shared_ptr<DataHolder>
load_sdl_rw_data(const std::filesystem::path& path)
{
  auto raw_data = load_binary(path).or_crash();
  pln::services::logger().info(
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

void
load_font(const std::filesystem::path& path)
{
  auto data = load_sdl_rw_data(path);
  auto result = tvg::Text::load(
    path.c_str(), data->payload().data(), data->payload().size(), "", true);
  vg_verify_or_crash(__func__, result);
}
