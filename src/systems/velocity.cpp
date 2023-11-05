#include <cmath>
#include <glm/geometric.hpp>

#include "../utils.h"
#include "../consts.h"
#include "velocity.h"


void velocity_system(GameState::Meta& meta) {
  meta.state.registry()
    .view<Position, Velocity>()
    .each([&](Position& position, Velocity& velocity) {
      velocity.velocity += velocity.acceleration * meta.seconds_since_last_frame;
      velocity.velocity = damp(
        velocity.velocity,
        {0.0, 0.0, 0.0},
        velocity.damping,
        meta.seconds_since_last_frame
      );
      position.value += velocity.velocity * meta.seconds_since_last_frame;
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
