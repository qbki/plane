#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/scene/scene.h"

#include "camera.h"

void
camera_move_system(Scene& scene)
{
  auto& camera = scene.state().camera();
  scene.state().registry().view<Transform, PlayerKind>().each(
    [&camera](const Transform& player_transform) {
      auto player_pos = player_transform.translation();
      camera.position({ player_pos.x, player_pos.y, camera.position().z });
    });
}
