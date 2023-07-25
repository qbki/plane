#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class Material {
private:
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;

public:
  explicit Material(const glm::vec3& color);

  glm::vec3 get_ambient() const;
  void set_ambient(const glm::vec3& value);

  glm::vec3 get_diffuse() const;
  void set_diffuse(const glm::vec3& value);

  glm::vec3 get_specular() const;
  void set_specular(const glm::vec3& value);

  float get_shininess() const;
  void set_shininess(float value);
};
