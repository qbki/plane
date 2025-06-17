module;
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "src/math/shapes.h"

export module pln.cameras.icamera;

namespace pln::cameras {

export
class ICamera
{
private:
  glm::mat4 _view;
  glm::mat4 _projection;
  glm::vec3 _position;
  glm::vec3 _forward_norm;
  float _near;
  float _far;

public:
  static constexpr float DEFAULT_NEAR { 0.1f };
  static constexpr float DEFAULT_FAR { 100.0f };
  static constexpr glm::vec3 DEFAULT_UP_NORM { 0, 1, 0 };

  virtual void screen_size(int width, int height) = 0;
  [[nodiscard]] virtual Shape::Frustum frustum() const = 0;

  ICamera(const ICamera&) = default;
  ICamera(ICamera&&) = delete;
  ICamera& operator=(const ICamera&) = default;
  ICamera& operator=(ICamera&&) = delete;
  virtual ~ICamera() = default;


  ICamera(float near = DEFAULT_NEAR, float far = DEFAULT_FAR)
    : _view(glm::mat4(1.0))
    , _projection(glm::mat4(1.0))
    , _position(glm::zero<glm::vec3>())
    , _forward_norm({ 0.0, 0.0, -1.0 })
    , _near(near)
    , _far(far)
  {
    look_at(glm::zero<glm::vec3>() + _forward_norm);
  }


  [[nodiscard]] glm::mat4
  pv() const
  {
    return _projection * _view;
  }


  void
  look_at(const glm::vec3& value)
  {
    _view = glm::lookAt(_position, value, DEFAULT_UP_NORM);
  }


  [[nodiscard]] glm::vec3
  position() const
  {
    return _position;
  }


  void
  position(const glm::vec3& value)
  {
    _position = value;
    look_at(value + _forward_norm);
  }


  [[nodiscard]] float
  far() const
  {
    return _far;
  }


  [[nodiscard]] float
  near() const
  {
    return _near;
  }


  [[nodiscard]] glm::vec3
  forward_norm() const
  {
    return _forward_norm;
  }


  void
  projection(const glm::mat4& matrix)
  {
    _projection = matrix;
  }


  [[nodiscard]] const glm::mat4&
  projection() const
  {
    return _projection;
  }


  [[nodiscard]] const glm::mat4&
  view() const
  {
    return _view;
  }
};

}
