module;
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/quaternion.hpp> // IWYU pragma: export
#include <glm/mat4x4.hpp>         // IWYU pragma: export
#include <glm/vec3.hpp>           // IWYU pragma: export

export module pln.components.transform;

namespace pln::components {

export
class Transform
{
private:
  glm::quat _rotation = glm::identity<glm::quat>();
  glm::vec3 _translation = glm::zero<glm::vec3>();
  glm::vec3 _scale { 1, 1, 1 };

  mutable glm::mat4 _matrix {};  // for cache purpose
  mutable bool _is_dirty = true; // for cache purpose

public:
  Transform&
  add_rotation_x(float radians)
  {
    _rotation *= glm::angleAxis(radians, glm::vec3(1, 0, 0));
    _is_dirty = true;
    return *this;
  }


  Transform&
  add_rotation_y(float radians)
  {
    _rotation *= glm::angleAxis(radians, glm::vec3(0, 1, 0));
    _is_dirty = true;
    return *this;
  }


  Transform&
  add_rotation_z(float radians)
  {
    _rotation *= glm::angleAxis(radians, glm::vec3(0, 0, 1));
    _is_dirty = true;
    return *this;
  }


  Transform&
  add_rotation(const glm::quat& value)
  {
    _rotation *= value;
    _is_dirty = true;
    return *this;
  }


  Transform&
  rotate(const glm::vec3& euler_rotation)
  {
    auto x = glm::angleAxis(euler_rotation.x, glm::vec3(1, 0, 0));
    auto y = glm::angleAxis(euler_rotation.y, glm::vec3(0, 1, 0));
    auto z = glm::angleAxis(euler_rotation.z, glm::vec3(0, 0, 1));
    _rotation = x * y * z;
    _is_dirty = true;
    return *this;
  }


  Transform&
  rotate(const glm::quat& rotation)
  {
    _rotation = rotation;
    _is_dirty = true;
    return *this;
  }


  Transform&
  rotate_z(float radians)
  {
    _rotation = glm::angleAxis(radians, glm::vec3(0, 0, 1));
    _is_dirty = true;
    return *this;
  }


  glm::vec3
  euler() const
  {
    return glm::eulerAngles(_rotation);
  }


  glm::quat
  rotation() const
  {
    return _rotation;
  }


  Transform&
  add_translation(const glm::vec3& value)
  {
    this->translate(_translation + value);
    _is_dirty = true;
    return *this;
  }


  Transform&
  translate(const glm::vec3& value)
  {
    _translation = value;
    _is_dirty = true;
    return *this;
  }


  glm::vec3
  translation() const
  {
    return _translation;
  }


  Transform&
  scale(const glm::vec3& value)
  {
    _scale = value;
    _is_dirty = true;
    return *this;
  }


  glm::mat4&
  matrix() const
  {
    if (_is_dirty) {
      auto identity = glm::mat4(1);
      auto translation_matrix = glm::translate(identity, _translation);
      auto rotation_matrix = glm::mat4_cast(_rotation);
      auto scale_matrix = glm::scale(identity, _scale);
      _matrix = translation_matrix * rotation_matrix * scale_matrix;
      _is_dirty = false;
    }
    return _matrix;
  }
};

}
