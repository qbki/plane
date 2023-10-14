#include "material.h"


Material::Material(glm::vec3 color) {
  this->_ambient = color * 0.03f;
  this->_specular = glm::vec3(0.5, 0.5, 0.5);
  this->_shininess = 30;
}


Material::Material(glm::vec3 ambient, glm::vec3 specular, float shininess) {
  this->_ambient = ambient;
  this->_specular = specular;
  this->_shininess = shininess;
}


glm::vec3 Material::ambient() const {
  return this->_ambient;
}


void Material::ambient(const glm::vec3& value) {
  this->_ambient = value;
}


glm::vec3 Material::specular() const {
  return this->_specular;
}


void Material::specular(const glm::vec3& value) {
  this->_specular = value;
}


float Material::shininess() const {
  return this->_shininess;
}


void Material::shininess(float value) {
  this->_shininess = value;
}
