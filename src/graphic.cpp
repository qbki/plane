#include <glm/ext/matrix_transform.hpp>

#include "./graphic.h"


glm::vec3 Graphic::get_position() const {
  return this->position;
}


void Graphic::set_position(const glm::vec3& position) {
  this->position = position;
  this->update_transform();
}


float Graphic::get_rotation_z() const {
  return this->rotation_z;
}


void Graphic::set_rotation_z(float rotation) {
  this->rotation_z = rotation;
  this->update_transform();
}


const glm::mat4& Graphic::get_transform() const {
  return this->transform;
}


void Graphic::set_transform(const glm::mat4& transform) {
  this->transform = transform;
}


void Graphic::add_child(std::shared_ptr<Graphic> child) {
  this->children.push_back(child);
}


void Graphic::draw(const Camera& camera, const SunLight& light, float elapsed_seconds) const {
  for (auto& child : children) {
    child->draw(camera, light, elapsed_seconds);
  }
}


void Graphic::move_in(glm::vec3 direction_normal, float length) {
  set_position(position + direction_normal * length);
}


void Graphic::update_transform() {
  auto rotation_matrix = glm::rotate(glm::mat4(1.0), this->rotation_z, {0.0, 0.0, 1.0});
  auto transform_matrix = glm::translate(glm::mat4(1.0), this->position);
  this->transform = transform_matrix * rotation_matrix;
}
