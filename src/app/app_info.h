#pragma once
#include <filesystem>
#include <optional>

struct AppInfo
{
  std::optional<std::filesystem::path> current_level{ std::nullopt };
  int hostiles{ 0 };
};
