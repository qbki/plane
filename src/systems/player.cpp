#include <glm/gtx/norm.hpp>

#include "./index.h"


void player_rotation_system(GameState::Meta& meta) {
  auto [position, rotation] = meta.state.player<Position, Rotation>();
  auto direction = meta.state.cursor() - position.value;
  auto angle = glm::atan(direction.y, direction.x);
  rotation.value = {0.0, 0.0, angle};
}


void player_shooting_system(GameState::Meta& meta) {
  if (!meta.control.is_player_shooting) {
    return;
  }

  auto player_position = meta.state.player<Position>().value;
  auto player_rotation = meta.state.player<Rotation>().value;
  auto projectiles_view = meta.state.registry().view<Position, ProjectileKind>(entt::exclude<Available>);
  auto max_spread = 3.14 / 16.0;
  auto spread = max_spread * (std::rand() % 100) * 0.01;
  auto rotation = glm::vec3(
    0.0,
    0.0,
    player_rotation.z - max_spread / 2.0 + spread
  );
  auto projectile_id = projectiles_view.front();
  if (projectile_id == entt::null) {
    meta.state.factory().make_projectile(meta.state.registry(), player_position, rotation);
  } else {
    meta.state.registry().emplace_or_replace<Position>(projectile_id, player_position);
    meta.state.registry().emplace_or_replace<InitialPosition>(projectile_id, player_position);
    meta.state.registry().emplace_or_replace<Rotation>(projectile_id, rotation);
    meta.state.registry().emplace_or_replace<Available>(projectile_id);
  }
};


void player_moving_system(GameState::Meta& meta) {
  glm::vec3 move_normal {0, 0, 0};
  if (meta.control.left) {
    move_normal.x -= 1;
  }
  if (meta.control.right) {
    move_normal.x += 1;
  }
  if (meta.control.up) {
    move_normal.y += 1;
  }
  if (meta.control.down) {
    move_normal.y -= 1;
  }
  auto direction = is_approx_equal(glm::length2(move_normal), 0.0f)
    ? move_normal
    : glm::normalize(move_normal);
  auto& position = meta.state.player<Position>();
  position.value = move_in(
    position.value,
    direction,
    5.0 * meta.seconds_since_last_frame
  );
}
