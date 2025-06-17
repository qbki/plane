#include <string>
#include <utility>

#include "src/components/common.h"
#include "src/gui/components/ui.h"
#include "src/gui/screens/settings_screen.h"

#include "in_game_main_menu_factory.h"
#include "utils/utils.h"

import pln.app.app;
import pln.scene.iscene;
import pln.services.app;
import pln.services.theme;

namespace GUI {

void
in_game_main_menu_factory(pln::scene::IScene& scene)
{
  auto ui = Factory::make_ui(scene.state().shared_registry());

  ui.block({
    .children = Children({
      ui.text_button({
        .text = "Resume",
        .on_pointer_down_once =
          [](auto&) {
            pln::services::app().add_once_handler([](pln::app::App& app) {
              app.scenes().pop_back();
              app.pause_scenes(false);
            });
          },
      }),

      ui.text_button({
        .text = "Settings",
        .on_pointer_down =
          [](auto&) {
            pln::services::app().add_once_handler([](auto&) {
              auto& scenes = pln::services::app().scenes();
              auto scenes_quantity = scenes.size();
              if (scenes_quantity > 0) {
                auto& scene = scenes[scenes_quantity - 1];
                scene->is_paused(true);
              }
              auto scene = load_settings_screen();
              pln::services::app().push_scene(std::move(scene));
            });
          },
      }),

      ui.text_button({
        .text = "Title screen",
        .on_pointer_down_once =
          [](auto&) { pln::services::app().add_once_handler(go_to_main_menu); },
      }),
    }),
  });

  ui.background({
    .color = pln::services::theme().components.menu_screen.background_color,
    .z = -0.1, // NOLINT
  });

  scene.cancel_handlers().add([](auto&) {
      pln::services::app().add_once_handler([](pln::app::App& app) {
      app.scenes().pop_back();
      app.pause_scenes(false);
    });
  });
}

}
