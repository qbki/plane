module;
#include <tuple>

export module pln.gui.loading_factory;

import pln.components.transform;
import pln.gui.components.text;
import pln.math.shapes;
import pln.scene.iscene;
import pln.services.app;
import pln.services.theme;

using namespace pln::components;

namespace pln::gui {

export
void
loading_factory(pln::scene::IScene& scene)
{
  auto& registry = scene.state().shared_registry();

  auto loading_text_entity = pln::gui::components::text(
    registry,
    {
      .font = pln::services::theme().typography.h1,
      .text = "Loading...",
    });
  scene.handlers().add([loading_text_entity](pln::scene::IScene& scene) {
    auto& registry = scene.state().registry();
    auto [transform,
          size] = registry.get<Transform, pln::math::RectSize>(loading_text_entity);
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
