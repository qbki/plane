module;
#include <optional>

export module pln.gui.utils;

import pln.app.app;

namespace pln::gui::utils {

export
void
clear_user_progress(pln::app::App& app)
{
  app.info().current_level = std::nullopt;
  app.save_data().save(app.assets_dir(), { .current_level = "" });
}

}
