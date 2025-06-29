module;
#include <glm/gtc/matrix_transform.hpp>

export module pln.cameras.gui_camera;

import pln.cameras.icamera;
import pln.math.shapes;

namespace pln::cameras {

export
class GUICamera : public ICamera
{
private:
  int _width;
  int _height;

public:
  GUICamera(int screen_width,
            int screen_height,
            float near = DEFAULT_NEAR,
            float far = DEFAULT_FAR)
    : ICamera(near, far)
    , _width(screen_width)
    , _height(screen_height)
  {
    nonvirt_screen_size(screen_width, screen_height);
  }


  void
  screen_size(int width, int height)
  {
    nonvirt_screen_size(width, height);
  }


  pln::math::Frustum
  frustum() const
  {
    // No need now
    return {};
  }


private:
  void
  nonvirt_screen_size(int width, int height)
  {
    _width = width;
    _height = height;
    ICamera::projection(glm::ortho(0.f,
                                   static_cast<float>(width),
                                   static_cast<float>(height),
                                   0.f,
                                   -1.f,
                                   1.f));
  }

};

}
