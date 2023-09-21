#include <glm/ext/quaternion_geometric.hpp>

#include "sun_light.h"


SunLight::SunLight(
  glm::vec3 _color,
  glm::vec3 _direction
) : _color(_color) {
  this->direction(_direction);
}


const glm::vec3 SunLight::color() const {
  return this->_color;
}


void SunLight::color(const glm::vec3& value) {
  this->_color = value;
}


const glm::vec3 SunLight::direction() const {
  return this->_direction;
}


void SunLight::direction(const glm::vec3& value) {
  this->_direction = glm::normalize(value);
}
