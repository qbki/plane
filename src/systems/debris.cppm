module;
#include <entt/entt.hpp>

export module pln.systems.debris;

import pln.components.common;
import pln.components.transform;
import pln.scene.iscene;

using namespace pln::components;

namespace pln::systems::debris {

export
void
remove_debris(pln::scene::IScene& scene)
{
  auto& registry = scene.state().registry();
  registry.view<const Transform, DebrisKind>().each(
    [&registry](entt::entity entity, const Transform& transform) {
      if (transform.translation().z < 0.0) {
        registry.destroy(entity);
      }
    });
}

}
