module;
#include <algorithm>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

export module pln.fileio.levels_order_loader;

import pln.services.app;
import pln.utils.file_loaders;

using namespace pln::utils::file_loaders;

namespace pln::fileio {

export
struct LevelsOrder
{
  std::vector<std::filesystem::path> levels;
};


std::filesystem::path
to_path(const std::string& path) {
  return pln::services::app().assets_dir() / path;
}


export
LevelsOrder
load_levels_order(const std::filesystem::path& levels_order_path)
{
  auto json = load_json(levels_order_path);
  auto files = json.or_crash().at("levels").get<std::vector<std::string>>();
  std::vector<std::filesystem::path> levels {};
  std::ranges::transform(files, std::back_inserter(levels), to_path);
  return { .levels = levels };
}

}
