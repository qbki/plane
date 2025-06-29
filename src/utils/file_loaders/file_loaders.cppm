module;
#include <SDL_mixer.h>
#include <SDL_rwops.h>
#include <filesystem>
#include <format>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <thorvg.h>
#include <tiny_gltf.h>
#include <utility>
#include <vector>

export module pln.utils.file_loaders;

import pln.services.logger;
import pln.sounds.sound;
import pln.utils.data_holder;
import pln.utils.result;
import pln.utils.tvg;

namespace pln::utils::file_loaders {

export
pln::utils::Result<tinygltf::Model>
load_gltf_model(const std::string& file_path);

export
pln::utils::Result<std::string>
load_text(const std::string& file_path);

export
pln::utils::Result<nlohmann::basic_json<>>
load_json(const std::string& file_path);

export
pln::utils::Result<nlohmann::basic_json<>>
load_local_json(const std::filesystem::path& file_path);

export
void
save_local_json(const std::filesystem::path& file_path, nlohmann::json json);

export
std::shared_ptr<pln::utils::data_holder::DataHolder>
load_sdl_rw_data(const std::filesystem::path& path);

export
std::unique_ptr<pln::sounds::Sound>
load_sound(const std::filesystem::path& sound_file_path);

export
void
load_font(const std::filesystem::path&);

export
bool
is_file_exists(const std::filesystem::path& path);

export
pln::utils::Result<std::vector<char>>
load_binary(const std::string& file_name);


export
std::shared_ptr<pln::utils::data_holder::DataHolder>
load_sdl_rw_data(const std::filesystem::path& path)
{
  using namespace pln::utils::data_holder;
  auto raw_data = load_binary(path).or_crash();
  pln::services::logger().info(
    std::format("Loaded: {} ({})", path.c_str(), raw_data.size()));
  return std::make_shared<DataHolder>(std::move(raw_data));
}


export
std::unique_ptr<pln::sounds::Sound>
load_sound(const std::filesystem::path& sound_file_path)
{
  auto holder = load_sdl_rw_data(sound_file_path);
  auto rwops = holder->rwops();
  auto chunk = Mix_LoadWAV_RW(rwops.get(), 0);
  return std::make_unique<pln::sounds::Sound>(chunk);
}


export
void
load_font(const std::filesystem::path& path)
{
  auto data = load_sdl_rw_data(path);
  auto result = tvg::Text::load(
    path.c_str(), data->payload().data(), data->payload().size(), "", true);
  vg_verify_or_crash(__func__, result);
}

}
