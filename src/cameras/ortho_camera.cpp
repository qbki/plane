#include <glm/ext/matrix_clip_space.hpp>
#include <glm/vec3.hpp>

#include "src/consts.h"
#include "src/math/shapes.h"

#include "camera.h"
#include "ortho_camera.h"

OrthoCamera::OrthoCamera(int screen_width,
                         int screen_height,
                         float near,
                         float far,
                         float scale_factor)
  : Camera(near, far)
  , _scale_factor(scale_factor)
{
  nonvirt_screen_size(screen_width, screen_height);
}

void
OrthoCamera::nonvirt_screen_size(int width, int height)
{
  _half_width = static_cast<float>(width) * HALF / _scale_factor;
  _half_height = static_cast<float>(height) * HALF / _scale_factor;
  auto ortho = glm::ortho(-_half_width,
                          _half_width,
                          -_half_height,
                          _half_height,
                          Camera::near(),
                          Camera::far());
  Camera::projection(ortho);
}

void
OrthoCamera::screen_size(int width, int height)
{
  nonvirt_screen_size(width, height);
}

Shape::Frustum
OrthoCamera::frustum() const
{
  auto near = Camera::near();
  auto far = Camera::far();
  auto forward_norm = Camera::forward_norm();
  auto position = Camera::position();
  return {
    .near {
      .point = position + forward_norm * near,
      .normal = forward_norm,
    },
    .far {
      .point = position + forward_norm * far,
      .normal = -forward_norm,
    },
    .left {
      .point = position + glm::vec3(-_half_width, 0, 0),
      .normal = { 1, 0, 0 },
    },
    .right {
      .point = position + glm::vec3(_half_width, 0, 0),
      .normal = { -1, 0, 0 },
    },
    .top {
      .point = position + glm::vec3(0, _half_height, 0),
      .normal = { 0, -1, 0 },
    },
    .bottom {
      .point = position + glm::vec3(0, -_half_height, 0),
      .normal = { 0, 1, 0 },
    },
  };
}
