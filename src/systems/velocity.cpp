#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/utils/interpolations.h"

#include "velocity.h"

import pln.consts;

void
acceleration_system(const Scene& scene)
{
  scene.state().registry().view<Velocity, Acceleration>().each(
    [&](Velocity& velocity, Acceleration& acceleration) {
      auto delta_time = Services::app().delta_time();
      velocity.value += acceleration.value * delta_time;
      acceleration.value = { 0, 0, 0 };
    });
}

void
damping_system(const Scene& scene)
{
  scene.state().registry().view<Velocity, VelocityDamping>().each(
    [&](Velocity& velocity, const VelocityDamping& damping) {
      auto delta_time = Services::app().delta_time();
      velocity.value = damp(velocity.value, damping.value, delta_time);
    });
}

void
velocity_system(const Scene& scene)
{
  scene.state().registry().view<Transform, Velocity>().each(
    [&](Transform& transform, const Velocity& velocity) {
      auto delta_time = Services::app().delta_time();
      transform.add_translation(velocity.value * delta_time);
    });
}

void
velocity_gravity_system(const Scene& scene)
{
  scene.state().registry().view<Acceleration, Gravity>().each(
    [&](Acceleration& acceleration) {
      acceleration.value += glm::vec3(0.0, 0.0, -pln::consts::GRAVITY);
    });
}
