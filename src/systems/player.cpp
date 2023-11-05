#include <glm/gtx/norm.hpp>

#include "./index.h"


void player_rotation_system(GameState::Meta& meta) {
  auto [position, rotation] = meta.state.player<Position, Rotation>();
  auto direction = meta.state.cursor() - position.value;
  auto angle = glm::atan(direction.y, direction.x);
  rotation.value = {0.0, 0.0, angle};
}


void player_shooting_system(GameState::Meta& meta) {
  auto& registry = meta.state.registry();

  if (!meta.control.is_player_shooting) {
    return;
  }

  auto [player_position, player_rotation] = meta.state.player<Position, Rotation>();
  auto projectiles_view = registry.view<ProjectileKind>(entt::exclude<Available>);
  auto max_spread = 3.14 / 16.0;
  auto spread = max_spread * (std::rand() % 100) * 0.01;
  auto rotation = glm::vec3(
    0.0,
    0.0,
    player_rotation.value.z - max_spread / 2.0 + spread
  );
  auto projectile_id = projectiles_view.front();
  if (projectile_id == entt::null) {
    meta.state.factory().make_projectile(registry, player_position.value, rotation);
  } else {
    auto move_direction = glm::vec3(glm::cos(rotation.z), glm::sin(rotation.z), 0.0);
    auto [
      prj_position, prj_initial_position, prj_rotation, prj_velocity, prj_speed
    ] = registry.get<
      Position, InitialPosition, Rotation, Velocity, Speed
    >(projectile_id);
    prj_position.value = player_position.value;
    prj_initial_position.value = player_position.value;
    prj_rotation.value = rotation;
    prj_velocity.velocity = move_direction * prj_speed.value;
    registry.emplace_or_replace<Available>(projectile_id);
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
  auto& velocity = meta.state.player<Velocity>();
  velocity.acceleration = direction * velocity.scalar_acceleration;
}
