#pragma once
#include <glm/matrix.hpp>

class Camera {
private:
  glm::mat4 view_matrix;
  glm::mat4 projection;
  glm::vec3 position;
  glm::vec3 forward_norm;
public:
  Camera(glm::vec3 _position, float aspect_ratio);
  glm::mat4 pv() const;

  void set_aspect_ratio(float aspect_ratio);

  void look_at(const glm::vec3& value);

  glm::vec3 get_position() const;
  void set_position(const glm::vec3& value);
};
