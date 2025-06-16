module;
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/scene/scene.h"

export module pln.systems.velocity;

import pln.consts;
import pln.services.app;
import pln.utils.interpolations;

namespace pln::systems::velocity {

export
void
acceleration(const Scene& scene)
{
  scene.state().registry().view<Velocity, Acceleration>().each(
    [&](Velocity& velocity, Acceleration& acceleration) {
      auto delta_time = pln::services::app().delta_time();
      velocity.value += acceleration.value * delta_time;
      acceleration.value = { 0, 0, 0 };
    });
}


export
void
damping(const Scene& scene)
{
  using namespace pln::utils::interpolations;

  scene.state().registry().view<Velocity, VelocityDamping>().each(
    [&](Velocity& velocity, const VelocityDamping& damping) {
      auto delta_time = pln::services::app().delta_time();
      velocity.value = damp(velocity.value, damping.value, delta_time);
    });
}


export
void
velocity(const Scene& scene)
{
  scene.state().registry().view<Transform, Velocity>().each(
    [&](Transform& transform, const Velocity& velocity) {
      auto delta_time = pln::services::app().delta_time();
      transform.add_translation(velocity.value * delta_time);
    });
}


export
void
velocity_gravity(const Scene& scene)
{
  scene.state().registry().view<Acceleration, Gravity>().each(
    [&](Acceleration& acceleration) {
      acceleration.value += glm::vec3(0.0, 0.0, -pln::consts::GRAVITY);
    });
}

}
