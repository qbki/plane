#include "sun_light.h"
#include <glm/ext/quaternion_geometric.hpp>


SunLight::SunLight(
  glm::vec3 _color,
  glm::vec3 _direction
) : color(_color) {
  this->set_direction(_direction);
}


const glm::vec3 SunLight::get_color() const {
  return this->color;
}


void SunLight::set_color(const glm::vec3& value) {
  this->color = value;
}


const glm::vec3 SunLight::get_direction() const {
  return this->direction;
}


void SunLight::set_direction(const glm::vec3& value) {
  this->direction = glm::normalize(value);
}
