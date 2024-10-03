#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include "src/consts.h"
#include "src/gui/core/font.h"
#include "src/gui/core/theme.h"
#include "src/utils/data_holder.h"
#include "src/utils/file_loaders.h"

#include "theme_loader.h"

std::unique_ptr<GUI::Theme>
load_theme(const std::filesystem::path& theme_file_path)
{
  auto json_theme = load_json(theme_file_path);
  std::filesystem::path font_path { json_theme.at("font").get<std::string>() };
  font_path = ASSETS_DIR / font_path;
  auto data_holder = load_sdl_rw_data(font_path);
  auto theme = std::make_unique<GUI::Theme>();

  using namespace GUI;
  auto make_font = [&data_holder](unsigned int size) {
    return std::make_shared<GUI::Font>(data_holder, size);
  };
  theme->typography.body1 = make_font(Typography::SIZE_BODY_1);
  theme->typography.body2 = make_font(Typography::SIZE_BODY_2);
  theme->typography.h1 = make_font(Typography::SIZE_H1);
  theme->typography.h2 = make_font(Typography::SIZE_H2);
  theme->typography.h3 = make_font(Typography::SIZE_H3);
  theme->typography.h4 = make_font(Typography::SIZE_H4);
  theme->typography.h5 = make_font(Typography::SIZE_H5);
  theme->typography.h6 = make_font(Typography::SIZE_H6);
  return theme;
}
