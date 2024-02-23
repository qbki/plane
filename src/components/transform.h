#pragma once
#include <glm/gtc/quaternion.hpp>

class Transform
{
private:
  glm::quat _rotation = glm::identity<glm::quat>();
  glm::vec3 _translation = glm::zero<glm::vec3>();

public:
  Transform& add_rotation_x(float radians);
  Transform& add_rotation_y(float radians);
  Transform& add_rotation_z(float radians);
  Transform& add_rotation(const glm::quat& value);
  Transform& rotate(const glm::vec3& euler_rotation);
  Transform& rotate(const glm::quat& rotation);
  Transform& rotate_z(float radians);
  [[nodiscard]] glm::vec3 euler() const;
  [[nodiscard]] glm::quat rotation() const;

  Transform& add_translation(const glm::vec3& value);
  Transform& translate(const glm::vec3& value);
  [[nodiscard]] glm::vec3 translation() const;

  [[nodiscard]] glm::mat4 matrix() const;
};
