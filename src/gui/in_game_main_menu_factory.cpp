#include <utility>

#include "src/components/common.h"
#include "src/gui/components/ui.h"
#include "src/gui/core/theme.h"
#include "src/gui/screens/settings_screen.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "in_game_main_menu_factory.h"
#include "utils/utils.h"

namespace GUI {

void
in_game_main_menu_factory(Scene& scene)
{
  auto ui = Factory::make_ui(scene.state().shared_registry());

  ui.block({
    .children = Children({
      ui.text_button({
        .text = "Resume",
        .on_pointer_down_once =
          [](auto&) {
            Services::app().add_once_handler([](App& app) {
              app.scenes().pop_back();
              app.pause_scenes(false);
            });
          },
      }),

      ui.text_button({
        .text = "Settings",
        .on_pointer_down =
          [](auto&) {
            Services::app().add_once_handler([](auto&) {
              auto& scenes = Services::app().scenes();
              auto scenes_quantity = scenes.size();
              if (scenes_quantity > 0) {
                auto& scene = scenes[scenes_quantity - 1];
                scene->is_paused(true);
              }
              auto scene = load_settings_screen();
              Services::app().push_scene(std::move(scene));
            });
          },
      }),

      ui.text_button({
        .text = "Title screen",
        .on_pointer_down_once =
          [](auto&) { Services::app().add_once_handler(setup_main_menu); },
      }),
    }),
  });

  ui.background({
    .color = Services::theme().components.menu_screen.background_color,
    .z = -0.1, // NOLINT
  });

  scene.cancel_handlers().add([](auto&) {
    Services::app().add_once_handler([](App& app) {
      app.scenes().pop_back();
      app.pause_scenes(false);
    });
  });
}

}
