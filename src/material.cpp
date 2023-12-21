#include "material.h"

const float Material::DEFAULT_AMBIENT_FRACTION = 0.03f;
const glm::vec3 Material::DEFAULT_SPECULAR = glm::vec3(0.5, 0.5, 0.5);
const float Material::DEFAULT_SHININESS = 30.f;

Material::Material(glm::vec3 color)
  : _ambient(color * Material::DEFAULT_AMBIENT_FRACTION)
  , _specular(Material::DEFAULT_SPECULAR)
  , _shininess(Material::DEFAULT_SHININESS)
{
}

Material::Material(glm::vec3 ambient, glm::vec3 specular, float shininess)
  : _ambient(ambient)
  , _specular(specular)
  , _shininess(shininess)
{
}

glm::vec3
Material::ambient() const
{
  return this->_ambient;
}

void
Material::ambient(const glm::vec3& value)
{
  this->_ambient = value;
}

glm::vec3
Material::specular() const
{
  return this->_specular;
}

void
Material::specular(const glm::vec3& value)
{
  this->_specular = value;
}

float
Material::shininess() const
{
  return this->_shininess;
}

void
Material::shininess(float value)
{
  this->_shininess = value;
}
