#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>

#include "camera.h"

const float Camera::DEFAULT_NEAR = 0.1f;
const float Camera::DEFAULT_FAR = 30.0f;
const glm::vec3 Camera::DEFAULT_UP_NORM = { 0, 1, 0 };

Camera::Camera(float near, float far)
  : _view(glm::mat4(1.0))
  , _projection(glm::mat4(1.0))
  , _position(glm::zero<glm::vec3>())
  , _forward_norm({ 0.0, 0.0, -1.0 })
  , _near(near)
  , _far(far)
{
  look_at(glm::zero<glm::vec3>() + _forward_norm);
}

glm::mat4
Camera::pv() const
{
  return this->_projection * this->_view;
}

glm::vec3
Camera::position() const
{
  return this->_position;
}

void
Camera::position(const glm::vec3& value)
{
  this->_position = value;
  this->look_at(value + this->_forward_norm);
}

float
Camera::far() const
{
  return _far;
}

float
Camera::near() const
{
  return _near;
}

glm::vec3
Camera::forward_norm() const
{
  return _forward_norm;
}

void
Camera::projection(const glm::mat4& matrix)
{
  _projection = matrix;
}

const glm::mat4&
Camera::projection() const
{
  return _projection;
}

const glm::mat4&
Camera::view() const
{
  return _view;
}

void
Camera::look_at(const glm::vec3& value)
{
  _view = glm::lookAt(_position, value, DEFAULT_UP_NORM);
}
