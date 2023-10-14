#include <glm/ext/quaternion_geometric.hpp>

#include "sun_light.h"


SunLight::SunLight(
  glm::vec3 color,
  glm::vec3 direction,
  glm::vec3 position
) : _color(color), _position(position) {
  this->direction(direction);
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


const glm::vec3 SunLight::position() const {
  return this->_position;
}


void SunLight::position(const glm::vec3& value) {
  this->_position = value;
}
