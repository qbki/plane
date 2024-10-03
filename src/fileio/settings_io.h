#pragma once
#include <filesystem>

struct SettingsData
{
  constexpr static int DEFAULT_VOLUME = 100;

  int volume = DEFAULT_VOLUME;
};

SettingsData
load_settings(const std::filesystem::path& settings_path);

void
save_settings(const std::filesystem::path& settings_path,
              const SettingsData& settings);
