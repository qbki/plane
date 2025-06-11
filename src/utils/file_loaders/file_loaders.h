#pragma once
#include <SDL_rwops.h> // IWYU pragma: export
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <tiny_gltf.h>

#include "src/sound/sound.h"
#include "src/utils/data_holder.h" // IWYU pragma: export
#include "src/utils/result.h"      // IWYU pragma: export

Result<tinygltf::Model>
load_gltf_model(const std::string& file_path);

Result<std::string>
load_text(const std::string& file_path);

Result<nlohmann::basic_json<>>
load_json(const std::string& file_path);

Result<nlohmann::basic_json<>>
load_local_json(const std::filesystem::path& file_path);

void
save_local_json(const std::filesystem::path& file_path, nlohmann::json json);

std::shared_ptr<DataHolder>
load_sdl_rw_data(const std::filesystem::path& path);

std::unique_ptr<Sound::Sound>
load_sound(const std::filesystem::path& sound_file_path);

void
load_font(const std::filesystem::path&);

bool
is_file_exists(const std::filesystem::path& path);
