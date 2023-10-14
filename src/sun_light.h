#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class SunLight {
private:
  glm::vec3 _color = glm::vec3(1.0, 1.0, 1.0);
  glm::vec3 _direction = glm::vec3(0.0, 0.0, 1.0);
  glm::vec3 _position = glm::vec3(0.0, 0.0, 100.0);

public:
  SunLight(glm::vec3 color, glm::vec3 direction, glm::vec3 position);

  const glm::vec3 color() const;
  void color(const glm::vec3& value);

  const glm::vec3 direction() const;
  void direction(const glm::vec3& value);

  const glm::vec3 position() const;
  void position(const glm::vec3& value);
};
