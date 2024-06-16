#define GLM_ENABLE_EXPERIMENTAL
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>
#include <string>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/utils/common.h"

void
player_rotation_system(Scene& scene)
{
  static const auto tilt_angle = glm::pi<float>() / 4.0f;
  scene.state().registry().view<Transform, Velocity, PlayerKind>().each(
    [&scene](auto& transform, const auto& velocity) {
      auto direction = scene.state().cursor() - transform.translation();
      auto x_tilt = -velocity.velocity.y / velocity.max_speed;
      auto y_tilt = velocity.velocity.x / velocity.max_speed;
      auto x = glm::angleAxis(tilt_angle * x_tilt, glm::vec3(1, 0, 0));
      auto y = glm::angleAxis(tilt_angle * y_tilt, glm::vec3(0, 1, 0));
      auto z =
        glm::angleAxis(glm::atan(direction.y, direction.x), glm::vec3(0, 0, 1));
      transform.rotate(x * y * z);
    });
}

void
player_shooting_system(const Scene& scene)
{
  const auto& control = Services::app().control();
  if (!control.is_player_shooting) {
    return;
  }
  scene.state()
    .registry()
    .view<ProjectileEmitter, ShotSound, PlayerKind>()
    .each(
      [](entt::entity entity, ProjectileEmitter& emitter, ShotSound& sound) {
        if (emitter.value.has_value()) {
          emitter.value.value()(entity);
        }
        if (sound.value.has_value()) {
          Services::events<const Events::ShootEvent>().emit(
            { sound.value.value() });
        }
      });
};

void
player_moving_system(const Scene& scene)
{
  const auto& control = Services::app().control();
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
  scene.state().registry().view<Velocity, PlayerKind>().each(
    [&direction](Velocity& velocity) {
      velocity.acceleration = direction * velocity.scalar_acceleration;
    });
}
