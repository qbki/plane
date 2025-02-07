#include <nlohmann/json.hpp>
#include <ranges>
#include <string>
#include <vector>

#include "src/services.h"
#include "src/utils/file_loaders.h"

#include "levels_order_loader.h"

LevelsOrder
load_levels_order(const std::filesystem::path& levels_order_path)
{
  auto json = load_json(levels_order_path);
  auto files = json.or_crash().at("levels").get<std::vector<std::string>>();
  auto files_view = files | std::views::transform([](const auto& path) {
                      return Services::app().assets_dir() / path;
                    });
  return { .levels { files_view.begin(), files_view.end() } };
}
