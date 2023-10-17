#pragma once
#include <glm/vec3.hpp>

class PointLight {
private:
  glm::vec3 _color = glm::vec3(1.0, 1.0, 1.0);
  glm::vec3 _position = glm::vec3(0.0, 0.0, 1.0);
  float _constant = 1.0;
  float _linear = 1.0;
  float _quadratic = 1.0;

public:
  PointLight(glm::vec3 color, glm::vec3 position);

  const glm::vec3 color() const;
  void color(const glm::vec3& value);

  const glm::vec3 position() const;
  void position(const glm::vec3& value);

  const float constant() const;
  void constant(float value);

  const float linear() const;
  void linear(float value);

  const float quadratic() const;
  void quadratic(float value);
};
