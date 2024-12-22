#define GLM_ENABLE_EXPERIMENTAL
#include <algorithm>
#include <cmath>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>

#include "src/utils/vectors.h"

#include "turret_rotation.h"

void
TurretRotation::speed(float value)
{
  _speed = value;
}

float
TurretRotation::speed() const
{
  return _speed;
}

void
TurretRotation::angle(float value)
{
  _angle = value;
}

float
TurretRotation::angle() const
{
  return _angle;
}

void
TurretRotation::rotate(const glm::vec3& forward,
                       const glm::vec3& sight,
                       float delta_time)
{
  auto norm_forward = glm::normalize(forward);
  auto norm_sight = glm::normalize(sight);
  auto actual_angle = glm::acos(
    glm::clamp(-1.0f, 1.0f, glm::dot(norm_forward, norm_sight)));
  auto actual_speed = std::min(actual_angle, _speed) * delta_time;
  auto is_right = is_looking_right_xy(norm_forward, norm_sight);
  _angle += is_right ? -actual_speed : actual_speed;
}

glm::quat
TurretRotation::quat() const
{
  return glm::angleAxis(_angle, glm::vec3(0, 0, 1));
}
