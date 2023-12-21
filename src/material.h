#pragma once
#include <glm/vec3.hpp>

class Material
{
private:
  glm::vec3 _ambient;
  glm::vec3 _specular;
  float _shininess;

public:
  static const float DEFAULT_AMBIENT_FRACTION;
  static const glm::vec3 DEFAULT_SPECULAR;
  static const float DEFAULT_SHININESS;

  explicit Material(glm::vec3 color);
  explicit Material(glm::vec3 ambient, glm::vec3 specular, float shininess);

  [[nodiscard]] glm::vec3 ambient() const;
  void ambient(const glm::vec3& value);

  [[nodiscard]] glm::vec3 specular() const;
  void specular(const glm::vec3& value);

  [[nodiscard]] float shininess() const;
  void shininess(float value);
};
