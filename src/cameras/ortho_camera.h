#pragma once
#include "camera.h"

class OrthoCamera : public Camera
{
private:
  float _half_width {};
  float _half_height {};
  float _scale_factor;

  constexpr static const float DEFAULT_SCALE_FACTOR = 40;

  void nonvirt_screen_size(int width, int height);

public:
  OrthoCamera(int screen_width,
              int screen_height,
              float near = DEFAULT_NEAR,
              float far = DEFAULT_FAR,
              float scale_factor = DEFAULT_SCALE_FACTOR);
  void screen_size(int width, int height) override;
  [[nodiscard]] Shape::Frustum frustum() const override;
};
