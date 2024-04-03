#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "src/math/shapes.h"

class Camera
{
private:
  glm::mat4 _view;
  glm::mat4 _projection;
  glm::vec3 _position;
  glm::vec3 _forward_norm;
  float _near;
  float _far;
  float _fovy;
  float _aspect_ratio;

public:
  static const float DEFAULT_FOVY;
  static const float DEFAULT_NEAR;
  static const float DEFAULT_FAR;
  static const glm::vec3 DEFAULT_UP_NORM;

  explicit Camera(float aspect_ratio,
                  float near = DEFAULT_NEAR,
                  float far = DEFAULT_FAR,
                  float fovy = DEFAULT_FOVY);

  [[nodiscard]] glm::mat4 pv() const;

  void aspect_ratio(float aspect_ratio);

  void look_at(const glm::vec3& value);

  [[nodiscard]] glm::vec3 position() const;
  void position(const glm::vec3& value);

  [[nodiscard]] const glm::mat4& projection() const;

  [[nodiscard]] const glm::mat4& view() const;

  [[nodiscard]] Shape::Frustum frustum() const;
};
