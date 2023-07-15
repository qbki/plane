#include "./material.h"


Material::Material(glm::vec3& color) {
  this->ambient = color * 0.1f;
  this->diffuse = color;
  this->specular = glm::vec3(0.1, 0.1, 0.1);
  this->shininess = 30;
}


glm::vec3 Material::get_ambient() const {
  return this->ambient;
}


void Material::set_ambient(const glm::vec3& value) {
  this->ambient = value;
}


glm::vec3 Material::get_diffuse() const {
  return this->diffuse;
}


void Material::set_diffuse(const glm::vec3& value) {
  this->diffuse = value;
}


glm::vec3 Material::get_specular() const {
  return this->specular;
}


void Material::set_specular(const glm::vec3& value) {
  this->specular = value;
}


float Material::get_shininess() const {
  return this->shininess;
}


void Material::set_shininess(float value) {
  this->shininess = value;
}
