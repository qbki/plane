export module pln.gui.lose_menu_factory;

import pln.app.app;
import pln.components.common;
import pln.events.event;
import pln.gui.components.ui;
import pln.gui.screens.main_menu_screen;
import pln.gui.utils;
import pln.scene.iscene;
import pln.services.app;
import pln.services.events;
import pln.services.theme;

using namespace pln::components;
using namespace pln::events;
using namespace pln::gui::components;
using namespace pln::gui::utils;

namespace pln::gui {

export
void
lose_menu_factory(const pln::scene::IScene& scene)
{
  auto ui = make_ui(scene.state().shared_registry());

  ui.block({
    .children = Children({
      ui.text_button({
        .text = "I will do it",
        .on_pointer_down_once =
          [](auto&) {
            pln::services::app().add_once_handler([](pln::app::App&) {
              pln::services::events<const LoadCurrentLevelEvent>().emit({});
            });
          },
      }),

      ui.text_button({
        .text = "Stop struggling",
        .on_pointer_down =
          [](auto&) { pln::services::app().add_once_handler(pln::gui::screens::go_to_main_menu); },
      }),

    }),
  });

  ui.background({
    .color = pln::services::theme().components.menu_screen.background_color,
    .z = -0.1, // NOLINT
  });
}

}
