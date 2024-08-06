#pragma once
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp> // IWYU pragma: export
#include <glm/mat4x4.hpp>         // IWYU pragma: export
#include <glm/vec3.hpp>           // IWYU pragma: export

class Transform
{
private:
  glm::quat _rotation = glm::identity<glm::quat>();
  glm::vec3 _translation = glm::zero<glm::vec3>();
  glm::vec3 _scale{ 1, 1, 1 };

  mutable glm::mat4 _matrix{};   // for cache purpose
  mutable bool _is_dirty = true; // for cache purpose

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
  Transform& scale(const glm::vec3& value);
  [[nodiscard]] glm::vec3 translation() const;

  [[nodiscard]] glm::mat4& matrix() const;
};
