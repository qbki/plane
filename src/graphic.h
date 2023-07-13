#pragma once
#include <glm/ext/matrix_transform.hpp>

class Graphic {
protected:
  glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
  glm::mat4 transform = glm::identity<glm::mat4>();

public:
  glm::vec3 get_position() const;
  void set_position(glm::vec3 position);

  const glm::mat4& get_transform() const;
  void set_transform(const glm::mat4& transform);
};
