#include <nlohmann/json.hpp>
#include <ranges>
#include <string>
#include <vector>

#include "src/consts.h"
#include "src/utils/file_loaders.h"
#include "src/utils/system.h"

#include "levels_order_loader.h"

LevelsOrder
load_levels_order(const std::filesystem::path& levels_order_path)
{
  auto json = load_json(levels_order_path);
  auto files = json.at("levels").get<std::vector<std::string>>();
  auto exec_path = get_excutable_path();
  auto files_view = files
                    | std::views::transform([&exec_path](const auto& path) {
                        return exec_path / ASSETS_DIR / path;
                      });
  return { .levels { files_view.begin(), files_view.end() } };
}
