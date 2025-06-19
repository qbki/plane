#include <filesystem>
#include <functional>
#include <nlohmann/json.hpp>

#include "src/utils/result.h"

#include "settings_io.h"

import pln.utils.file_loaders;

using namespace pln::utils::file_loaders;

SettingsData
load_settings(const std::filesystem::path& settings_path)
{
  auto load_settings = [](const nlohmann::basic_json<>& json) {
    return Result<SettingsData>::from_payload({
      .volume = json.value("volume", SettingsData::DEFAULT_VOLUME),
    });
  };
  return load_local_json(settings_path)
    .then<SettingsData>(load_settings)
    .or_fallback({});
}

void
save_settings(const std::filesystem::path& settings_path,
              const SettingsData& settings)
{
  nlohmann::json json;
  json["volume"] = settings.volume;
  save_local_json(settings_path, json);
}
