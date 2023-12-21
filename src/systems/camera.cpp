#include "./camera.h"
#include "../components.h"

void
camera_move_system(const App::Meta& meta)
{
  auto camera = meta.app->game_state->camera();
  auto [player_pos] = meta.app->game_state->player<Position>();
  camera->position({ player_pos.x, player_pos.y, camera->position().z });
}
