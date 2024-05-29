#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>

#include "src/app.h"
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/utils/common.h"

const auto TILT_ANGLE = glm::pi<float>() / 4.0f;

void
player_rotation_system(const App::Meta& meta)
{
  auto [transform, velocity] =
    meta.app->game_state->player<Transform, Velocity>();
  auto direction = meta.app->game_state->cursor() - transform.translation();
  auto x_tilt = -velocity.velocity.y / velocity.max_speed;
  auto y_tilt = velocity.velocity.x / velocity.max_speed;
  auto x = glm::angleAxis(TILT_ANGLE * x_tilt, glm::vec3(1, 0, 0));
  auto y = glm::angleAxis(TILT_ANGLE * y_tilt, glm::vec3(0, 1, 0));
  auto z =
    glm::angleAxis(glm::atan(direction.y, direction.x), glm::vec3(0, 0, 1));
  transform.rotate(x * y * z);
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
  glm::vec3 move_direction{ 0, 0, 0 };
  if (control.left) {
    move_direction.x -= 1;
  }
  if (control.right) {
    move_direction.x += 1;
  }
  if (control.up) {
    move_direction.y += 1;
  }
  if (control.down) {
    move_direction.y -= 1;
  }
  auto direction = is_approx_equal(glm::length2(move_direction), 0.0f)
                     ? move_direction
                     : glm::normalize(move_direction);
  auto& velocity = meta.app->game_state->player<Velocity>();
  velocity.acceleration = direction * velocity.scalar_acceleration;
}
