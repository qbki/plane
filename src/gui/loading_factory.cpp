#include "src/components/transform.h"
#include "src/scene/scene.h"

#include "components/text.h"

import pln.services.app;
import pln.services.theme;
import pln.shapes;

namespace GUI {

void
loading_factory(Scene& scene)
{
  auto& registry = scene.state().shared_registry();

  auto loading_text_entity = Factory::text(
    registry,
    {
      .font = pln::services::theme().typography.h1,
      .text = "Loading...",
    });
  scene.handlers().add([loading_text_entity](Scene& scene) {
    auto& registry = scene.state().registry();
    auto [transform,
          size] = registry.get<Transform, pln::shapes::RectSize>(loading_text_entity);
    const int offset = 16;
    auto screen_size = pln::services::app().screen_size();
    transform.translate({
      screen_size.width - size.width - offset,
      offset,
      transform.translation().z,
    });
  });
}

}
