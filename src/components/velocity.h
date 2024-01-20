#pragma once
#include <glm/vec3.hpp>

struct Velocity
{
  float scalar_acceleration;
  float damping;
  glm::vec3 acceleration;
  glm::vec3 velocity;

  Velocity();
  Velocity(float _acceleration, float _damping);
  Velocity(glm::vec3 _initial_velocity, float _damping);
};
