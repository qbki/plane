#include <filesystem>
#include <format>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

#include "src/consts.h"
#include "src/gui/font.h"
#include "src/gui/theme.h"
#include "src/utils/file_loaders.h"

#include "theme_loader.h"

std::unique_ptr<GUI::Theme>
load_theme(const std::filesystem::path& theme_file_path)
{
  auto json_theme = load_json(theme_file_path);
  std::filesystem::path font_path{ json_theme.at("font").get<std::string>() };
  font_path = ASSETS_DIR / font_path;
  auto rw_data = load_sdl_rw_data(font_path);
  auto theme = std::make_unique<GUI::Theme>();
  theme->typography.body_1 =
    std::make_shared<GUI::Font>(rw_data, GUI::Typography::SIZE_BODY_1);
  return theme;
}
