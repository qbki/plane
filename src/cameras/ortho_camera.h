#pragma once
#include "src/math/shapes.h"

#include "camera.h"

class OrthoCamera : public Camera
{
private:
  void nonvirt_screen_size(int width, int height);

public:
  OrthoCamera(int screen_width,
              int screen_height,
              float near = DEFAULT_NEAR,
              float far = DEFAULT_FAR);
  void screen_size(int width, int height) override;
  [[nodiscard]] Shape::Frustum frustum() const override;
};
