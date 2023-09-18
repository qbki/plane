#pragma once
#include <glm/matrix.hpp>

class Camera {
private:
  glm::mat4 _view;
  glm::mat4 _projection;
  glm::vec3 _position;
  glm::vec3 _forward_norm;
public:
  Camera(glm::vec3 _position, float aspect_ratio);
  glm::mat4 pv() const;

  void aspect_ratio(float aspect_ratio);

  void look_at(const glm::vec3& value);

  glm::vec3 position() const;
  void position(const glm::vec3& value);

  const glm::mat4& projection() const;

  const glm::mat4& view() const;
};
