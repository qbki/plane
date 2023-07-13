#pragma once
#include <glm/vec3.hpp>

class SunLight {
private:
  glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
  glm::vec3 direction = glm::vec3(0.0, 0.0, 1.0);

public:
  SunLight(glm::vec3 _color, glm::vec3 _direction);

  const glm::vec3 get_color() const;
  void set_color(const glm::vec3& value);

  const glm::vec3 get_direction() const;
  void set_direction(const glm::vec3& value);
};
