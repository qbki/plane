#include "src/components/transform.h"
#include "src/gui/core/theme.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "components/text.h"
#include "types.h"

namespace GUI {

void
loading_factory(Scene& scene)
{
  auto& registry = scene.state().registry();

  auto loading_text_entity = GUI::Factory::make_text(registry, "Loading...");
  registry.replace<GUI::FontPtr>(loading_text_entity,
                                 Services::theme().typography.h1);
  scene.handlers().add([loading_text_entity](Scene& scene) {
    auto& registry = scene.state().registry();
    auto [transform, button_size] =
      registry.get<Transform, RectSize>(loading_text_entity);
    const int offset = 16;
    auto screen_size = Services::app().screen_size();
    transform.translate({
      screen_size.width - button_size.width - offset,
      offset,
      transform.translation().z,
    });
  });
}

}
