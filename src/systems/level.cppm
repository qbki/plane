module;
#include <glm/common.hpp>

export module pln.systems.level;

import pln.components.common;
import pln.components.transform;
import pln.scene.iscene;

using namespace pln::components;

namespace pln::systems::level {

export
void
level_boundaries(const pln::scene::IScene& scene)
{
  auto& world_bbox = scene.state().world_bbox();

  scene.state().registry().view<Transform, Velocity, PlayerKind>().each(
    [&](Transform& transform, Velocity& velocity) {
      auto pos = transform.translation();
      if (pos.x > world_bbox.max.x || pos.x < world_bbox.min.x) {
        velocity.value.x = 0;
      }
      if (pos.y > world_bbox.max.y || pos.y < world_bbox.min.y) {
        velocity.value.y = 0;
      }
      if (pos.z > world_bbox.max.z || pos.z < world_bbox.min.z) {
        velocity.value.z = 0;
      }
      transform.translate({
        glm::clamp(pos.x, world_bbox.min.x, world_bbox.max.x),
        glm::clamp(pos.y, world_bbox.min.y, world_bbox.max.y),
        glm::clamp(pos.z, world_bbox.min.z, world_bbox.max.z),
      });
    });
}

}
