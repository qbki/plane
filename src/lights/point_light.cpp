#include <glm/geometric.hpp>

#include "point_light.h"


PointLight::PointLight(
  glm::vec3 color,
  glm::vec3 position
) : _color(color),
    _position(position)
{}


const glm::vec3 PointLight::color() const {
  return this->_color;
}


void PointLight::color(const glm::vec3& value) {
  this->_color = value;
}


const glm::vec3 PointLight::position() const {
  return this->_position;
}


void PointLight::position(const glm::vec3& value) {
  this->_position = value;
}


const float PointLight::constant() const {
  return this->_constant;
}


void PointLight::constant(float value) {
  this->_constant = value;
}


const float PointLight::linear() const {
  return this->_linear;
}


void PointLight::linear(float value) {
  this->_linear = value;
}


const float PointLight::quadratic() const {
  return this->_quadratic;
}


void PointLight::quadratic(float value) {
  this->_quadratic = value;
}
