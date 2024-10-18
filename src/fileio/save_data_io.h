#pragma once
#include <filesystem>
#include <optional>

struct SaveData
{
  std::optional<std::filesystem::path> current_level = std::nullopt;
};

SaveData
load_save_data(const std::filesystem::path& path);

void
store_save_data(const std::filesystem::path& path, const SaveData& save_data);
