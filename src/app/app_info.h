#pragma once
#include <filesystem>
#include <optional>

#include "src/components/common.h"

struct AppInfo
{
  std::optional<std::filesystem::path> current_level { std::nullopt };
  int hostiles { 0 };
  Lives lives {};
};
