#include "src/components/velocity.h"
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/consts.h"
#include "src/utils/interpolations.h"

#include "velocity.h"

void
velocity_system(const App& app)
{
  app.game_state().registry().view<Transform, Velocity>().each(
    [&](Transform& transform, Velocity& velocity) {
      auto delta_time = app.delta_time();
      velocity.velocity += velocity.acceleration * delta_time;
      velocity.velocity = damp(velocity.velocity, velocity.damping, delta_time);
      transform.add_translation(velocity.velocity * delta_time);
      velocity.acceleration = glm::vec3(0.0);
    });
}

void
velocity_gravity_system(const App& app)
{
  app.game_state().registry().view<Velocity, Gravity>().each(
    [&](Velocity& velocity) {
      velocity.acceleration += glm::vec3(0.0, 0.0, -GRAVITY);
    });
}
