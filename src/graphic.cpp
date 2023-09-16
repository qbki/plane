#include <glm/ext/matrix_transform.hpp>

#include "./graphic.h"


void Graphic::set_position(glm::vec3 position) {
  this->position = position;
  this->transform = glm::translate(glm::mat4(1.0), position);
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


void Graphic::add_child(std::shared_ptr<Graphic> child) {
  children.push_back(child);
}


void Graphic::draw(const Camera& camera, const SunLight& light, float elapsed_seconds) const {
  for (auto& child : children) {
    child->draw(camera, light, elapsed_seconds);
  }
}


void Graphic::move_in(glm::vec3 direction_normal, float length) {
  set_position(position + direction_normal * length);
}
