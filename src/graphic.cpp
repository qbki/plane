#include <glm/ext/matrix_transform.hpp>

#include "./graphic.h"


glm::vec3 Graphic::position() const {
  return this->_position;
}


void Graphic::position(const glm::vec3& position) {
  this->_position = position;
  this->update_transform();
}


float Graphic::rotation_z() const {
  return this->_rotation_z;
}


void Graphic::rotation_z(float rotation) {
  this->_rotation_z = rotation;
  this->update_transform();
}


const glm::mat4& Graphic::transform() const {
  return this->_transform;
}


void Graphic::transform(const glm::mat4& transform) {
  this->_transform = transform;
}


bool Graphic::is_active() const {
  return this->_is_active;
}


void Graphic::is_active(bool value) {
  this->_is_active = value;
}


glm::vec3 Graphic::velocity() const {
  return this->_velocity;
}


void Graphic::velocity(glm::vec3 velocity) {
  this->_velocity = velocity;
}


void Graphic::add_child(std::shared_ptr<Graphic> child) {
  this->_children.push_back(child);
}


void Graphic::draw(const Camera& camera, const SunLight& light, float elapsed_seconds) const {
  for (auto& child : _children) {
    if (this->is_active()) {
      child->draw(camera, light, elapsed_seconds);
    }
  }
}


void Graphic::move_in(glm::vec3 direction_normal, float length) {
  position(_position + direction_normal * length);
}


void Graphic::update_transform() {
  auto rotation_matrix = glm::rotate(glm::mat4(1.0), this->_rotation_z, {0.0, 0.0, 1.0});
  auto transform_matrix = glm::translate(glm::mat4(1.0), this->_position);
  this->_transform = transform_matrix * rotation_matrix;
}
