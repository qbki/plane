#include "src/components/transform.h"

#include "camera.h"

void
camera_move_system(const App::Meta& meta)
{
  auto camera = meta.app->game_state->camera();
  auto player_transform = meta.app->game_state->player<Transform>();
  auto player_pos = player_transform.translation();
  camera->position({ player_pos.x, player_pos.y, camera->position().z });
}
