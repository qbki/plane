#pragma once
#include <glm/vec3.hpp>

class DirectionalLight {
private:
  glm::vec3 _color = glm::vec3(1.0, 1.0, 1.0);
  glm::vec3 _direction = glm::vec3(0.0, 0.0, 1.0);

public:
  DirectionalLight(glm::vec3 color, glm::vec3 direction);

  const glm::vec3 color() const;
  void color(const glm::vec3& value);

  const glm::vec3 direction() const;
  void direction(const glm::vec3& value);
};
