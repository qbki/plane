#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>

#include "src/app/app.h"
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/events/event.h"
#include "src/services.h"
#include "src/utils/common.h"

const auto TILT_ANGLE = glm::pi<float>() / 4.0f;

void
player_rotation_system(const App& app)
{
  auto [transform, velocity] = app.game_state().player<Transform, Velocity>();
  auto direction = app.game_state().cursor() - transform.translation();
  auto x_tilt = -velocity.velocity.y / velocity.max_speed;
  auto y_tilt = velocity.velocity.x / velocity.max_speed;
  auto x = glm::angleAxis(TILT_ANGLE * x_tilt, glm::vec3(1, 0, 0));
  auto y = glm::angleAxis(TILT_ANGLE * y_tilt, glm::vec3(0, 1, 0));
  auto z =
    glm::angleAxis(glm::atan(direction.y, direction.x), glm::vec3(0, 0, 1));
  transform.rotate(x * y * z);
}

void
player_shooting_system(const App& app)
{
  if (!app.control().is_player_shooting) {
    return;
  }
  const auto& [emit_projectile, shot_sound] =
    app.game_state().player<ProjectileEmitter, ShotSound>();
  if (emit_projectile.value.has_value()) {
    emit_projectile.value.value()();
  }
  if (shot_sound.value.has_value()) {
    events<Events::ShootEvent>().emit({ shot_sound.value.value() });
  }
};

void
player_moving_system(const App& app)
{
  const auto& control = app.control();
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
  auto& velocity = app.game_state().player<Velocity>();
  velocity.acceleration = direction * velocity.scalar_acceleration;
}
