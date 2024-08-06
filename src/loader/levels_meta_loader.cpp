#include <nlohmann/json.hpp>
#include <ranges>
#include <string>
#include <vector>

#include "levels_meta_loader.h"
#include "src/consts.h"
#include "src/utils/file_loaders.h"

LevelsMeta
load_levels_meta(const std::filesystem::path& levels_meta_path)
{
  auto json = load_json(levels_meta_path);
  auto files = json.at("levels").get<std::vector<std::string>>();
  auto files_view = files | std::views::transform([](const auto& path) {
                      return LEVELS_DIR / path;
                    });
  return { .levels{ files_view.begin(), files_view.end() } };
}
