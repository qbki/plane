module;
#include "src/components/common.h"
#include "src/components/transform.h"

export module pln.systems.debris;

import pln.scene.iscene;

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
