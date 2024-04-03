#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "camera.h"

const float Camera::DEFAULT_FOVY = glm::radians(45.0f);
const float Camera::DEFAULT_NEAR = 0.1f;
const float Camera::DEFAULT_FAR = 30.0f;
const glm::vec3 Camera::DEFAULT_UP_NORM = { 0, 1, 0 };

Camera::Camera(float aspect_ratio, float near, float far, float fovy)
  : _view(glm::mat4(1.0))
  , _projection(glm::mat4(1.0))
  , _position(glm::zero<glm::vec3>())
  , _forward_norm({ 0.0, 0.0, -1.0 })
  , _near(near)
  , _far(far)
  , _fovy(fovy)
  , _aspect_ratio(aspect_ratio)
{
  this->look_at(glm::zero<glm::vec3>() + _forward_norm);
  this->aspect_ratio(aspect_ratio);
}

void
Camera::aspect_ratio(float aspect_ratio)
{
  this->_aspect_ratio = aspect_ratio;
  this->_projection = glm::perspective(DEFAULT_FOVY, aspect_ratio, _near, _far);
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
  this->_view = glm::lookAt(this->_position, value, DEFAULT_UP_NORM);
}

Shape::Frustum
Camera::frustum() const
{
  const auto half_vertical = _far * glm::tan(_fovy * 0.5f);
  const auto half_horizontal = half_vertical * _aspect_ratio;
  const auto right_norm =
    glm::normalize(glm::cross(_forward_norm, DEFAULT_UP_NORM));
  const auto central_far_point = _position + _forward_norm * _far;
  return {
    .near{
      .point = _position + _forward_norm * _near,
      .normal = _forward_norm,
    },
    .far{
      .point = _position + _forward_norm * _far,
      .normal = -_forward_norm,
    },
    .left{
      .point = _position,
      .normal = glm::normalize(glm::cross(
        DEFAULT_UP_NORM,
        _position - (central_far_point - half_horizontal * right_norm))),
    },
    .right{
      .point = _position,
      .normal = glm::normalize(glm::cross(
        _position - (central_far_point + half_horizontal * right_norm),
        DEFAULT_UP_NORM)),
    },
    .top{
      .point = _position,
      .normal = glm::normalize(glm::cross(
        right_norm,
        _position - (central_far_point + half_vertical * DEFAULT_UP_NORM))),
    },
    .bottom{
      .point = _position,
      .normal = glm::normalize(glm::cross(
        _position - (central_far_point - half_vertical * DEFAULT_UP_NORM),
        right_norm)),
    },
  };
}
