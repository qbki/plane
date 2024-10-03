#include <filesystem>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "src/services.h"
#include "src/utils/file_loaders.h"

#include "settings_io.h"

SettingsData
load_settings(const std::filesystem::path& settings_path)
{
  try {
    auto settings = load_local_json(settings_path);
    return {
      .volume = settings.value("volume", SettingsData::DEFAULT_VOLUME),
    };
  } catch (const std::runtime_error& error) {
    Services::logger().warn("Can't load a settings file, used default values.");
    Services::logger().warn(error.what());
  }
  return {};
}

void
save_settings(const std::filesystem::path& settings_path,
              const SettingsData& settings)
{
  nlohmann::json json;
  json["volume"] = settings.volume;
  save_local_json(settings_path, json);
}
