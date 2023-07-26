#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "./camera.h"


Camera::Camera(
  glm::vec3 _position, float aspect_ratio
) : position(_position) {
  auto zero = glm::vec3(0.0, 0.0, 0.0);
  this->look_at(zero);
  this->set_aspect_ratio(aspect_ratio);
}


void Camera::set_aspect_ratio(float aspect_ratio) {
  this->projection = glm::perspective(
    glm::radians(45.0f),
    aspect_ratio,
    0.1f,
    100.0f
  );
}


glm::mat4 Camera::pv() const {
  return this->projection * this->view_matrix;
}


glm::vec3 Camera::get_position() const {
  return this->position;
}


void Camera::set_position(const glm::vec3& value) {
  this->position = value;
}


void Camera::look_at(const glm::vec3& value) {
  this->view_matrix = glm::lookAt(
    this->position,
    value,
    glm::vec3(0.0, 1.0, 0.0)
  );
}
