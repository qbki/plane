#include <glm/common.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/scene/scene.h"

void
level_boundaries_system(const Scene& scene)
{
  auto& world_bbox = scene.state().world_bbox();

  scene.state().registry().view<Transform, PlayerKind>().each(
    [&](Transform& transform) {
      auto pos = transform.translation();
      transform.translate({
        glm::clamp(pos.x, world_bbox.min.x, world_bbox.max.x),
        glm::clamp(pos.y, world_bbox.min.y, world_bbox.max.y),
        glm::clamp(pos.z, world_bbox.min.z, world_bbox.max.z),
      });
    });
}
