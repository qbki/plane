module;
#include <tuple>

export module pln.gui.settings_screen_factory;

import pln.components.common;
import pln.components.percent;
import pln.gui.components.ui;
import pln.scene.iscene;
import pln.services.app;
import pln.services.theme;

using namespace pln::components;
using namespace pln::gui::components;

namespace pln::gui {

export
void
settings_screen_factory(pln::scene::IScene& scene)
{
  auto& registry = scene.state().shared_registry();
  auto ui = make_ui(registry);
  const int step = 5;

  auto progress = ui.progress({
    .width = 200, // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    .height = 20, // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    .value = pln::services::app().settings().master_volume().value(),
  });

  auto make_volume_changer = [registry, progress](int step) {
    return [registry, progress, step](auto&) {
      auto& settings = pln::services::app().settings();
      settings.master_volume().add(step);
      auto [percent,
            is_percent_dirty] = registry->get<Percent, IsDirty>(progress);
      percent.value(settings.master_volume().value());
      is_percent_dirty.value = true;
    };
  };

  ui.block({
    .children = Children({
      ui.text({
        .color = pln::services::theme().components.text.color,
        .font = pln::services::theme().typography.h2,
        .text = "Settings",
      }),

      ui.row({
        .children = Children({
          ui.text({
            .color = pln::services::theme().components.text.color,
            .font = pln::services::theme().typography.h4,
            .text = "Sound: ",
          }),

          ui.text_button({
            .text = "<",
            .on_pointer_down = make_volume_changer(-step),
          }),

          progress,

          ui.text_button({
            .text = ">",
            .on_pointer_down = make_volume_changer(step),
          }),
        }),
      }),

      ui.text_button({
        .text = "Back",
        .on_pointer_down =
          [](auto&) {
            pln::services::app().add_once_handler([](auto&) {
              auto& app = pln::services::app();
              app.pop_scene();
              auto scenes_quantity = app.scenes().size();
              if (scenes_quantity > 0) {
                app.scenes()[scenes_quantity - 1]->is_paused(false);
              }
              app.settings().save();
            });
          },
      }),
    }),
  });

  ui.background({
    .color = pln::services::theme().components.menu_screen.background_color,
    .z = -0.1, // NOLINT
  });
}

}
