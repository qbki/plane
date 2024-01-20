#include <glm/gtc/constants.hpp>

#include "velocity.h"

Velocity::Velocity()
  : scalar_acceleration(0)
  , damping(0)
  , acceleration(glm::zero<glm::vec3>())
  , velocity(glm::zero<glm::vec3>())
{
}

Velocity::Velocity(float _acceleration, float _damping)
  : scalar_acceleration(_acceleration)
  , damping(_damping)
  , acceleration(glm::vec3(0.0))
  , velocity(glm::vec3(0.0))
{
}

Velocity::Velocity(glm::vec3 _initial_velocity, float _damping)
  : scalar_acceleration(0)
  , damping(_damping)
  , acceleration(glm::vec3(0.0))
  , velocity(_initial_velocity)
{
}
