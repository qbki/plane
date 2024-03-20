#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
private:
  glm::mat4 _view;
  glm::mat4 _projection;
  glm::vec3 _position;
  glm::vec3 _forward_norm;

public:
  static const float DEFAULT_FOVY;
  static const float DEFAULT_Z_NEAR;
  static const float DEFAULT_Z_FAR;

  explicit Camera(float aspect_ratio);

  [[nodiscard]] glm::mat4 pv() const;

  void aspect_ratio(float aspect_ratio);

  void look_at(const glm::vec3& value);

  [[nodiscard]] glm::vec3 position() const;
  void position(const glm::vec3& value);

  [[nodiscard]] const glm::mat4& projection() const;

  [[nodiscard]] const glm::mat4& view() const;
};
