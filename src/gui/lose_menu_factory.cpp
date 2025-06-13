#include <string>

#include "src/components/common.h"
#include "src/gui/components/ui.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "lose_menu_factory.h"
#include "utils/utils.h"

namespace GUI {

void
lose_menu_factory(const Scene& scene)
{
  auto ui = Factory::make_ui(scene.state().shared_registry());

  ui.block({
    .children = Children({
      ui.text_button({
        .text = "I will do it",
        .on_pointer_down_once =
          [](auto&) {
            Services::app().add_once_handler([](App&) {
              Services::events<const Events::LoadCurrentLevelEvent>().emit({});
            });
          },
      }),

      ui.text_button({
        .text = "Stop struggling",
        .on_pointer_down =
          [](auto&) { Services::app().add_once_handler(go_to_main_menu); },
      }),

    }),
  });

  ui.background({
    .color = Services::theme().components.menu_screen.background_color,
    .z = -0.1, // NOLINT
  });
}

}
