#include <optional>
#include <utility>
#include <vector>

#include "src/gui/screens/main_menu_screen.h"

#include "utils.h"

import pln.app.app;

namespace GUI {

void
go_to_main_menu(pln::app::App& app)
{
  app.scenes().clear();
  auto scene = load_main_menu();
  app.push_scene(std::move(scene));
}

void
clear_user_progress(pln::app::App& app)
{
  app.info().current_level = std::nullopt;
  app.save_data().save(app.assets_dir(), { .current_level = "" });
}

}
