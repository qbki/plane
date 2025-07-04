module;
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/vec3.hpp>

export module pln.cameras.ortho_camera;

import pln.cameras.icamera;
import pln.consts;
import pln.math.shapes;

namespace pln::cameras {

export
class OrthoCamera : public ICamera
{
private:
  float _half_width {0};
  float _half_height {0};
  float _scale_factor {1};

public:
  constexpr static const float DEFAULT_SCALE_FACTOR = 40;

  OrthoCamera(int screen_width,
              int screen_height,
              float near = DEFAULT_NEAR,
              float far = DEFAULT_FAR,
              float scale_factor = DEFAULT_SCALE_FACTOR)
    : ICamera(near, far)
    , _scale_factor(scale_factor)
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
    auto near = ICamera::near();
    auto far = ICamera::far();
    auto forward_norm = ICamera::forward_norm();
    auto position = ICamera::position();
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


private:
  void
  nonvirt_screen_size(int width, int height)
  {
    _half_width = static_cast<float>(width) * pln::consts::HALF / _scale_factor;
    _half_height = static_cast<float>(height) * pln::consts::HALF / _scale_factor;
    auto ortho = glm::ortho(-_half_width,
                            _half_width,
                            -_half_height,
                            _half_height,
                            ICamera::near(),
                            ICamera::far());
    ICamera::projection(ortho);
  }
};

}
