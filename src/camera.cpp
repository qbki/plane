#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>

#include "./camera.h"


Camera::Camera(
  glm::vec3 _position, float aspect_ratio
) : _position(_position) {
  auto zero = glm::vec3(0.0, 0.0, 0.0);
  this->_forward_norm = glm::normalize(zero - this->_position);
  this->look_at(zero);
  this->aspect_ratio(aspect_ratio);
}


void Camera::aspect_ratio(float aspect_ratio) {
  this->_projection = glm::perspective(
    glm::radians(45.0f),
    aspect_ratio,
    0.1f,
    100.0f
  );
}


glm::mat4 Camera::pv() const {
  return this->_projection * this->_view;
}


glm::vec3 Camera::position() const {
  return this->_position;
}


void Camera::position(const glm::vec3& value) {
  this->_position = value;
  this->look_at(value + this->_forward_norm);
}


const glm::mat4& Camera::projection() const {
  return this->_projection;
}


const glm::mat4& Camera::view() const {
  return this->_view;
}


void Camera::look_at(const glm::vec3& value) {
  this->_view = glm::lookAt(
    this->_position,
    value,
    glm::vec3(0.0, 1.0, 0.0)
  );
}
