#include "src/components/common.h"
#include "src/components/transform.h"

#include "debris.h"

void
remove_debris_system(Scene& scene)
{
  auto& registry = scene.state().registry();
  registry.view<const Transform, DebrisKind>().each(
    [&registry](entt::entity entity, const Transform& transform) {
      if (transform.translation().z < 0.0) {
        registry.destroy(entity);
      }
    });
}
