#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class Material {
private:
  glm::vec3 _ambient;
  glm::vec3 _specular;
  float _shininess;

public:
  explicit Material(const glm::vec3& color);

  glm::vec3 ambient() const;
  void ambient(const glm::vec3& value);

  glm::vec3 specular() const;
  void specular(const glm::vec3& value);

  float shininess() const;
  void shininess(float value);
};
