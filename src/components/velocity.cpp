#include <cmath>
#include <glm/ext/quaternion_exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

#include "velocity.h"

/**
 * Calc an aproximately maximum speed. Should be used with the "damp" function.
 */
float
calc_approx_max_speed(float acceleration, float damping)
{
  // Used https://www.desmos.com/ for visualization.
  //
  // The main formula used for calculating speed and damping:
  // F\left(x\right)=a\int_{0}^{x}s^{t}dt
  // a - acceleration
  // s - smoothing
  //
  // Solving of the integral:
  // f\left(x\right)=\frac{a\cdot s^{x}}{\ln\left(s\right)}
  //
  // The final result:
  // y=f\left(1000\right)-f\left(0\right)
  auto f = [&](float x) {
    return acceleration * glm::pow(damping, x) / std::log10(damping);
  };
  // The number was selected based on the graph where it becomes flat.
  // Any sane value of more than 3 gives an approximately same result.
  const float end_of_itegral = 3.f;
  return f(end_of_itegral) - f(0.f);
}

Velocity::Velocity()
  : scalar_acceleration(0)
  , damping(0)
  , max_speed(0)
  , acceleration(glm::zero<glm::vec3>())
  , velocity(glm::zero<glm::vec3>())
{
}

Velocity::Velocity(float _acceleration, float _damping)
  : scalar_acceleration(_acceleration)
  , damping(_damping)
  , max_speed(calc_approx_max_speed(_acceleration, _damping))
  , acceleration(glm::vec3(0.0))
  , velocity(glm::vec3(0.0))
{
}

Velocity::Velocity(glm::vec3 _initial_velocity, float _damping)
  : scalar_acceleration(0)
  , damping(_damping)
  , max_speed(glm::length(_initial_velocity))
  , acceleration(glm::vec3(0.0))
  , velocity(_initial_velocity)
{
}
