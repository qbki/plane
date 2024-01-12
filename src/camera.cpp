#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

#include "camera.h"
#include "utils/index.h"

const float Camera::DEFAULT_FOVY = 45.0f;
const float Camera::DEFAULT_Z_NEAR = 0.1f;
const float Camera::DEFAULT_Z_FAR = 100.0f;

Camera::Camera(float aspect_ratio)
  : _view(glm::mat4(1.0))
  , _projection(glm::mat4(1.0))
  , _position(zero<glm::vec3>())
  , _forward_norm({ 0.0, 0.0, -1.0 })
{
  this->look_at(zero<glm::vec3>() + _forward_norm);
  this->aspect_ratio(aspect_ratio);
}

void
Camera::aspect_ratio(float aspect_ratio)
{
  this->_projection = glm::perspective(
    glm::radians(DEFAULT_FOVY), aspect_ratio, DEFAULT_Z_NEAR, DEFAULT_Z_FAR);
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

const glm::mat4&
Camera::projection() const
{
  return this->_projection;
}

const glm::mat4&
Camera::view() const
{
  return this->_view;
}

void
Camera::look_at(const glm::vec3& value)
{
  this->_view = glm::lookAt(this->_position, value, glm::vec3(0.0, 1.0, 0.0));
}
