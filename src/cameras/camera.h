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

public:
  static const float DEFAULT_NEAR;
  static const float DEFAULT_FAR;
  static const glm::vec3 DEFAULT_UP_NORM;

  Camera(float near = DEFAULT_NEAR, float far = DEFAULT_FAR);
  Camera(const Camera&) = default;
  Camera(Camera&&) = delete;
  Camera& operator=(const Camera&) = default;
  Camera& operator=(Camera&&) = delete;
  virtual ~Camera() = default;

  [[nodiscard]] glm::mat4 pv() const;

  virtual void screen_size(int width, int height) = 0;

  void look_at(const glm::vec3& value);

  [[nodiscard]] glm::vec3 position() const;
  void position(const glm::vec3& value);

  [[nodiscard]] float far() const;
  [[nodiscard]] float near() const;
  [[nodiscard]] glm::vec3 forward_norm() const;

  void projection(const glm::mat4& matrix);
  [[nodiscard]] const glm::mat4& projection() const;

  [[nodiscard]] const glm::mat4& view() const;

  [[nodiscard]] virtual Shape::Frustum frustum() const = 0;
};
