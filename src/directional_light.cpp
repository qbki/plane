#include <glm/ext/quaternion_geometric.hpp>

#include "directional_light.h"


DirectionalLight::DirectionalLight(
  glm::vec3 color,
  glm::vec3 direction
) : _color(color) {
  this->direction(direction);
}


const glm::vec3 DirectionalLight::color() const {
  return this->_color;
}


void DirectionalLight::color(const glm::vec3& value) {
  this->_color = value;
}


const glm::vec3 DirectionalLight::direction() const {
  return this->_direction;
}


void DirectionalLight::direction(const glm::vec3& value) {
  this->_direction = glm::normalize(value);
}
