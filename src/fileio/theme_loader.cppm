module;
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>

export module pln.fileio.theme_loader;

import pln.fileio.json.mappers;
import pln.fileio.params.theme;
import pln.gui.core.theme;
import pln.services.app;
import pln.utils.file_loaders;

using namespace pln::fileio::params;
using namespace pln::utils::file_loaders;
using namespace pln::gui::core;

namespace pln::fileio {

export
std::unique_ptr<Theme>
load_theme(const std::filesystem::path& theme_file_path)
{
  auto theme_params = load_json(theme_file_path).or_crash().get<ThemeParams>();
  auto font_path = (pln::services::app().assets_dir() / theme_params.font_source)
                     .lexically_normal();
  load_font(font_path);

  auto theme = std::make_unique<Theme>();
  theme->font_path = font_path;
  theme->typography.body1.name = theme_params.font_family;
  theme->typography.body2.name = theme_params.font_family;
  theme->typography.h1.name = theme_params.font_family;
  theme->typography.h2.name = theme_params.font_family;
  theme->typography.h3.name = theme_params.font_family;
  theme->typography.h4.name = theme_params.font_family;
  theme->typography.h5.name = theme_params.font_family;
  theme->typography.h6.name = theme_params.font_family;
  return theme;
}

}
