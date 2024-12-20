#include "src/components/common.h"
#include "src/components/percent.h"
#include "src/gui/components/ui.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/utils/color.h"

#include "settings_screen_factory.h"

namespace GUI {

void
settings_screen_factory(Scene& scene)
{
  auto& registry = scene.state().shared_registry();
  auto ui = Factory::make_ui(registry);
  const int step = 5;

  auto progress = ui.progress({
    .width = 200, // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    .height = 20, // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    .value = Services::app().settings().master_volume().value(),
  });

  auto make_volume_changer = [registry, progress](int step) {
    return [registry, progress, step](auto&) {
      auto& settings = Services::app().settings();
      settings.master_volume().add(step);
      auto& percent = registry->get<Percent>(progress);
      percent.value(settings.master_volume().value());
    };
  };

  ui.block({
    .children = Children({
      ui.text({
        .color = Services::theme().components.text.color,
        .font = Services::theme().typography.h2,
        .text = "Settings",
      }),

      ui.row({
        .children = Children({
          ui.text({
            .color = Services::theme().components.text.color,
            .font = Services::theme().typography.h4,
            .text = "Sound: ",
          }),

          ui.text_button({
            .text = "< ",
            .on_pointer_down = make_volume_changer(-step),
          }),

          progress,

          ui.text_button({
            .text = " >",
            .on_pointer_down = make_volume_changer(step),
          }),
        }),
      }),

      ui.text_button({
        .text = "Back",
        .on_pointer_down =
          [](auto&) {
            Services::app().add_once_handler([](auto&) {
              auto& app = Services::app();
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
    .color = Services::theme().components.menu_screen.background_color,
    .z = -0.1, // NOLINT
  });
}

}
