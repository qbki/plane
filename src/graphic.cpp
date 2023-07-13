#include <glm/ext/matrix_transform.hpp>

#include "./graphic.h"


void Graphic::set_position(glm::vec3 position) {
  this->position = position;
  this->transform = glm::translate(this->transform, position);
}


glm::vec3 Graphic::get_position() const {
  return this->position;
}


const glm::mat4& Graphic::get_transform() const {
  return this->transform;
}


void Graphic::set_transform(const glm::mat4& transform) {
  this->transform = transform;
}
