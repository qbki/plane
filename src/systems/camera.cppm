module;
#include "src/components/transform.h"

export module pln.systems.camera;

import pln.components.common;
import pln.scene.iscene;

using namespace pln::components;

namespace pln::systems::camera {

export
void
camera_movement(pln::scene::IScene& scene)
{
  auto& camera = scene.state().camera();
  scene.state().registry().view<Transform, PlayerKind>().each(
    [&camera](const Transform& player_transform) {
      auto player_pos = player_transform.translation();
      camera.position({ player_pos.x, player_pos.y, camera.position().z });
    });
}

}
