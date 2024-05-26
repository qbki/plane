#include <glm/ext/matrix_clip_space.hpp>
#include <stdexcept>

#include "camera.h"
#include "ortho_camera.h"

OrthoCamera::OrthoCamera(int screen_width,
                         int screen_height,
                         float near,
                         float far)
  : Camera(near, far)
{
  nonvirt_screen_size(screen_width, screen_height);
}

void
OrthoCamera::nonvirt_screen_size(int width, int height)
{
  const float half_width = static_cast<float>(width) * 0.5f;
  const float half_height = static_cast<float>(height) * 0.5f;
  Camera::projection(
    glm::ortho(-half_width, half_width, -half_height, half_height, -1.f, 1.f));
}

void
OrthoCamera::screen_size(int width, int height)
{
  nonvirt_screen_size(width, height);
}

Shape::Frustum
OrthoCamera::frustum() const
{
  throw std::runtime_error("Not implemented");
}
