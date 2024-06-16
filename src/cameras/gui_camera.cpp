#include <glm/ext/matrix_clip_space.hpp>

#include "camera.h"
#include "gui_camera.h"

GUICamera::GUICamera(int screen_width, int screen_height, float near, float far)
  : Camera(near, far)
  , _width(screen_width)
  , _height(screen_height)
{
  nonvirt_screen_size(screen_width, screen_height);
}

void
GUICamera::nonvirt_screen_size(int width, int height)
{
  _width = width;
  _height = height;
  Camera::projection(glm::ortho(0.f,
                                static_cast<float>(width),
                                0.f,
                                static_cast<float>(height),
                                -1.f,
                                1.f));
}

void
GUICamera::screen_size(int width, int height)
{
  nonvirt_screen_size(width, height);
}

Shape::Frustum
GUICamera::frustum() const
{
  // No need now
  return {};
}
