#pragma once
#include "camera.h"

class GUICamera : public Camera
{
private:
  int _width;
  int _height;

  void nonvirt_screen_size(int width, int height);

public:
  GUICamera(int screen_width,
            int screen_height,
            float near = DEFAULT_NEAR,
            float far = DEFAULT_FAR);
  void screen_size(int width, int height) override;
  [[nodiscard]] Shape::Frustum frustum() const override;
};
