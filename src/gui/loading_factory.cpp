#include "src/components/transform.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "components/text.h"

namespace GUI {

void
loading_factory(Scene& scene)
{
  auto& registry = scene.state().shared_registry();

  auto loading_text_entity = Factory::text(
    registry,
    {
      .font = Services::theme().typography.h1,
      .text = "Loading...",
    });
  scene.handlers().add([loading_text_entity](Scene& scene) {
    auto& registry = scene.state().registry();
    auto [transform,
          size] = registry.get<Transform, RectSize>(loading_text_entity);
    const int offset = 16;
    auto screen_size = Services::app().screen_size();
    transform.translate({
      screen_size.width - size.width - offset,
      offset,
      transform.translation().z,
    });
  });
}

}
