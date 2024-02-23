#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "transform.h"

Transform&
Transform::add_rotation_x(float radians)
{
  _rotation *= glm::angleAxis(radians, glm::vec3(1, 0, 0));
  return *this;
}

Transform&
Transform::add_rotation_y(float radians)
{
  _rotation *= glm::angleAxis(radians, glm::vec3(0, 1, 0));
  return *this;
}

Transform&
Transform::add_rotation_z(float radians)
{
  _rotation *= glm::angleAxis(radians, glm::vec3(0, 0, 1));
  return *this;
}

Transform&
Transform::add_rotation(const glm::quat& value)
{
  _rotation *= value;
  return *this;
}

Transform&
Transform::rotate(const glm::vec3& euler_rotation)
{
  auto x = glm::angleAxis(euler_rotation.x, glm::vec3(1, 0, 0));
  auto y = glm::angleAxis(euler_rotation.y, glm::vec3(0, 1, 0));
  auto z = glm::angleAxis(euler_rotation.z, glm::vec3(0, 0, 1));
  _rotation = x * y * z;
  return *this;
}

Transform&
Transform::rotate(const glm::quat& rotation)
{
  _rotation = rotation;
  return *this;
}

Transform&
Transform::rotate_z(float radians)
{
  _rotation = glm::angleAxis(radians, glm::vec3(0, 0, 1));
  return *this;
}

glm::vec3
Transform::euler() const
{
  return glm::eulerAngles(_rotation);
}

glm::quat
Transform::rotation() const
{
  return _rotation;
}

Transform&
Transform::add_translation(const glm::vec3& value)
{
  this->translate(_translation + value);
  return *this;
}

Transform&
Transform::translate(const glm::vec3& value)
{
  _translation = value;
  return *this;
}

glm::vec3
Transform::translation() const
{
  return _translation;
}

glm::mat4
Transform::matrix() const
{
  auto translation_matrix = glm::translate(glm::mat4(1), _translation);
  auto rotation_matrix = glm::mat4_cast(_rotation);
  return translation_matrix * rotation_matrix;
}
