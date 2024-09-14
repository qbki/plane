#include "src/app/settings.h"
#include "src/components/common.h"
#include "src/components/percent.h"
#include "src/gui/components/ui.h"
#include "src/gui/core/theme.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "settings_screen_factory.h"

namespace GUI {

void
settings_screen_factory(Scene& scene)
{
  auto& registry = scene.state().shared_registry();
  auto ui = Factory::make_ui(registry);
  const float step = 0.1;
  auto volume = Services::app().settings().master_volume().value();

  auto progress = ui.progress({
    .width = 200, // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    .height = 20, // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    .value = static_cast<float>(volume),
  });

  auto make_volume_changer = [registry, progress](float step) {
    return [registry, progress, step](auto&) {
      auto& settings = Services::app().settings();
      settings.master_volume().add(step);
      auto& percent = registry->get<Percent>(progress);
      percent.value(settings.master_volume().value());
    };
  };

  ui.block(
    {
      .children =
        Children({
          ui.text({
            .color = Services::theme().components.text.color,
            .font = Services::theme().typography.h2,
            .text = "Settings",
          }),

          ui.row({
            .children = Children({
              ui.text_button({
                .text = "< ",
                .on_pointer_down = make_volume_changer(-step),
              }),

              progress,

              ui.text_button(
                { .text = " >", .on_pointer_down = make_volume_changer(step) }),
            }),
          }),

          ui.text_button(
            {
              .text = "Back",
              .on_pointer_down =
                [](auto&) {
                  Services::app().add_once_handler([](auto&) {
                    Services::app().pop_scene();
                    for (auto& scene : Services::app().scenes()) {
                      scene->is_paused(false);
                    }
                  });
                },
            }),
        }),
    });
}

}
