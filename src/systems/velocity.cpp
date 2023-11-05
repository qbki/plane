#include <cmath>
#include <glm/geometric.hpp>

#include "../utils.h"
#include "../consts.h"
#include "velocity.h"


void velocity_system(GameState::Meta& meta) {
  meta.state.registry()
    .view<Position, Velocity>()
    .each([&](Position& position, Velocity& velocity) {
      velocity.velocity += velocity.acceleration * meta.delta_time;
      velocity.velocity = damp(
        velocity.velocity,
        {0.0, 0.0, 0.0},
        velocity.damping,
        meta.delta_time
      );
      position.value += velocity.velocity * meta.delta_time;
      velocity.acceleration = glm::vec3(0.0);
    });
}

void velocity_gravity_system(GameState::Meta& meta) {
  meta.state.registry()
    .view<Velocity, Gravity>()
    .each([&](Velocity& velocity) {
      velocity.acceleration += glm::vec3(0.0, 0.0, -GRAVITY);
    });
}
