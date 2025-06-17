module;
#include "src/components/common.h"
#include "src/components/transform.h"

export module pln.systems.camera;

import pln.scene.iscene;

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
