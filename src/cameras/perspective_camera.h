#pragma once
#include "src/math/shapes.h"

#include "camera.h"

class PerspectiveCamera : public Camera
{
private:
  float _fovy;
  float _aspect_ratio;

  void nonvirt_screen_size(int width, int height);

public:
  static const float DEFAULT_FOVY;

  PerspectiveCamera(int screen_width,
                    int screen_height,
                    float near = DEFAULT_NEAR,
                    float far = DEFAULT_FAR,
                    float fovy = DEFAULT_FOVY);
  void screen_size(int width, int height) override;
  [[nodiscard]] Shape::Frustum frustum() const override;
};
