#pragma once
#include <filesystem>
#include <vector>

struct LevelsOrder
{
  std::vector<std::filesystem::path> levels;
};

LevelsOrder
load_levels_order(const std::filesystem::path& levels_order_path);
