#include <glm/gtx/norm.hpp>

#include "../components.h"
#include "./index.h"

void
player_rotation_system(const App::Meta& meta)
{
  auto [position, rotation] =
    meta.app->game_state->player<Position, Rotation>();
  auto direction = meta.app->game_state->cursor() - position.value;
  auto angle = glm::atan(direction.y, direction.x);
  rotation.value = { 0.0, 0.0, angle };
}

void
player_shooting_system(const App::Meta& meta)
{
  if (!meta.app->control->is_player_shooting) {
    return;
  }
  auto emit_projectile = meta.app->game_state->player<ProjectileEmitter>();
  emit_projectile();
};

void
player_moving_system(const App::Meta& meta)
{
  const auto& control = *meta.app->control;
  glm::vec3 move_normal{ 0, 0, 0 };
  if (control.left) {
    move_normal.x -= 1;
  }
  if (control.right) {
    move_normal.x += 1;
  }
  if (control.up) {
    move_normal.y += 1;
  }
  if (control.down) {
    move_normal.y -= 1;
  }
  auto direction = is_approx_equal(glm::length2(move_normal), 0.0f)
                     ? move_normal
                     : glm::normalize(move_normal);
  auto& velocity = meta.app->game_state->player<Velocity>();
  velocity.acceleration = direction * velocity.scalar_acceleration;
}
