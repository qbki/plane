#include "src/components/velocity.h"
#include "src/components/common.h"
#include "src/components/linear_velocity.h"
#include "src/components/transform.h"
#include "src/consts.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/utils/interpolations.h"

#include "velocity.h"

void
velocity_system(const Scene& scene)
{
  scene.state().registry().view<Transform, Velocity>().each(
    [&](Transform& transform, Velocity& velocity) {
      auto delta_time = Services::app().delta_time();
      velocity.velocity += velocity.acceleration * delta_time;
      velocity.velocity = damp(velocity.velocity, velocity.damping, delta_time);
      transform.add_translation(velocity.velocity * delta_time);
      velocity.acceleration = glm::vec3(0.0);
    });
}

void
linear_velocity_system(const Scene& scene)
{
  auto delta_time = Services::app().delta_time();
  scene.state().registry().view<Transform, LinearVelocity>().each(
    [&](Transform& transform, const LinearVelocity& velocity) {
      transform.add_translation(velocity.velocity * delta_time);
    });
}

void
velocity_gravity_system(const Scene& scene)
{
  scene.state().registry().view<Velocity, Gravity>().each(
    [&](Velocity& velocity) {
      velocity.acceleration += glm::vec3(0.0, 0.0, -GRAVITY);
    });

  auto delta_time = -GRAVITY * Services::app().delta_time();
  scene.state().registry().view<LinearVelocity, Gravity>().each(
    [&](LinearVelocity& velocity) {
      velocity.velocity += glm::vec3(0.0, 0.0, delta_time);
    });
}
