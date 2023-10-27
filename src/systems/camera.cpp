#include "../components.h"
#include "./camera.h"


void camera_move_system(GameState::Meta& meta) {
  auto [player_pos] = meta.state.player<Position>();
  auto camera_pos = meta.state.camera()->position();
  meta.state.camera()->position({player_pos.x, player_pos.y, camera_pos.z});
}
