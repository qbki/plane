#include <glm/ext/matrix_clip_space.hpp>
#include <stdexcept>

#include "camera.h"
#include "gui_camera.h"

GUICamera::GUICamera(int screen_width, int screen_height, float near, float far)
  : Camera(near, far)
{
  nonvirt_screen_size(screen_width, screen_height);
}

void
GUICamera::nonvirt_screen_size(int width, int height)
{
  Camera::projection(glm::ortho(0.f,
                                static_cast<float>(width),
                                -static_cast<float>(height),
                                0.f,
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
  throw std::runtime_error("Not implemented");
}
