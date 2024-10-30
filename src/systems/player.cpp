#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/weapon.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/utils/common.h"

void
player_rotation_system(Scene& scene)
{
  static const auto tilt_angle = glm::pi<float>() / 4.0f;
  static const auto max_speed_coeficient = 0.4f;
  scene.state()
    .registry()
    .view<Transform, Velocity, AccelerationScalar, PlayerKind>()
    .each([&](Transform& transform,
              const Velocity& velocity,
              const AccelerationScalar& accel) {
      auto direction = scene.state().cursor() - transform.translation();
      float x_tilt = 0;
      float y_tilt = 0;
      auto max_speed = accel.value * max_speed_coeficient;
      if (max_speed > 0) {
        x_tilt = -velocity.value.y / max_speed;
        y_tilt = velocity.value.x / max_speed;
      }
      auto x = glm::angleAxis(tilt_angle * x_tilt, glm::vec3(1, 0, 0));
      auto y = glm::angleAxis(tilt_angle * y_tilt, glm::vec3(0, 1, 0));
      auto z = glm::angleAxis(glm::atan(direction.y, direction.x),
                              glm::vec3(0, 0, 1));
      transform.rotate(x * y * z);
    });
}

void
player_shooting_system(const Scene& scene)
{
  const auto& control = Services::app().control();
  scene.state().registry().view<Weapon, PlayerKind>().each(
    [&control](Weapon& weapon) { weapon.is_shooting = control.shooting; });
};

void
player_moving_system(const Scene& scene)
{
  const auto& control = Services::app().control();
  glm::vec3 move_direction { 0, 0, 0 };
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
  scene.state()
    .registry()
    .view<Acceleration, AccelerationScalar, PlayerKind>()
    .each([&direction](Acceleration& accel,
                       const AccelerationScalar& accel_scalar) {
      accel.value += direction * accel_scalar.value;
    });
}
