module;
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/scene/scene.h"

export module pln.systems.debris;

namespace pln::systems::debris {

export
void
remove_debris(Scene& scene)
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
