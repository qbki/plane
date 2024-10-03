#pragma once
#include <filesystem>
#include <vector>

struct LevelsMeta
{
  std::vector<std::filesystem::path> levels;
};

LevelsMeta
load_levels_meta(const std::filesystem::path& levels_meta_path);
