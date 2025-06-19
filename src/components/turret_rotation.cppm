module;
#define GLM_ENABLE_EXPERIMENTAL
#include <algorithm>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

export module pln.components.turret_rotation;

import pln.utils.vectors;

namespace pln::components {

export
class TurretRotation
{
private:
  float _speed { 0 };
  float _angle { 0 };

public:
  void
  speed(float value)
  {
    _speed = value;
  }


  [[nodiscard]] float
  speed() const
  {
    return _speed;
  }


  void
  angle(float value)
  {
    _angle = value;
  }


  [[nodiscard]] float
  angle() const
  {
    return _angle;
  }

  void
  rotate(const glm::vec3& forward,
         const glm::vec3& sight,
         float delta_time)
  {
    using namespace pln::utils::vectors;

    auto norm_forward = glm::normalize(forward);
    auto norm_sight = glm::normalize(sight);
    auto actual_angle = glm::acos(
      glm::clamp(-1.0f, 1.0f, glm::dot(norm_forward, norm_sight)));
    auto actual_speed = std::min(actual_angle, _speed) * delta_time;
    auto is_right = is_looking_right_xy(norm_forward, norm_sight);
    _angle += is_right ? -actual_speed : actual_speed;
  }

  [[nodiscard]] glm::quat
  quat() const
  {
    return glm::angleAxis(_angle, glm::vec3(0, 0, 1));
  }

};

}
