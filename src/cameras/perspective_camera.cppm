module;
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "src/math/shapes.h"

export module pln.cameras.perspective_camera;

import pln.cameras.icamera;

namespace pln::cameras {

export
class PerspectiveCamera : public ICamera
{
private:
  float _fovy;
  float _aspect_ratio;

public:
  static constexpr float DEFAULT_FOVY { glm::radians(45.0f) };

  PerspectiveCamera(int screen_width,
                    int screen_height,
                    float near = DEFAULT_NEAR,
                    float far = DEFAULT_FAR,
                    float fovy = DEFAULT_FOVY)
    : ICamera(near, far)
    , _fovy(fovy)
    , _aspect_ratio(calc_aspect_ratio(screen_width, screen_height))
  {
    nonvirt_screen_size(screen_width, screen_height);
  }


  void
  screen_size(int width, int height)
  {
    nonvirt_screen_size(width, height);
  }


  Shape::Frustum
  frustum() const
  {
    auto near = ICamera::near();
    auto far = ICamera::far();
    auto forward_norm = ICamera::forward_norm();
    auto position = ICamera::position();

    const auto half_vertical = far * glm::tan(_fovy * 0.5f);
    const auto half_horizontal = half_vertical * _aspect_ratio;
    const auto right_norm = glm::normalize(
      glm::cross(forward_norm, DEFAULT_UP_NORM));
    const auto central_far_point = position + forward_norm * far;
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
        .point = position,
        .normal = glm::normalize(glm::cross(
          DEFAULT_UP_NORM,
          position - (central_far_point - half_horizontal * right_norm))),
      },
      .right {
        .point = position,
        .normal = glm::normalize(glm::cross(
          position - (central_far_point + half_horizontal * right_norm),
          DEFAULT_UP_NORM)),
      },
      .top {
        .point = position,
        .normal = glm::normalize(glm::cross(
          right_norm,
          position - (central_far_point + half_vertical * DEFAULT_UP_NORM))),
      },
      .bottom {
        .point = position,
        .normal = glm::normalize(glm::cross(
          position - (central_far_point - half_vertical * DEFAULT_UP_NORM),
          right_norm)),
      },
    };
  }


private:
  float
  calc_aspect_ratio(int width, int height)
  {
    return static_cast<float>(width) / static_cast<float>(height);
  }

  void
  nonvirt_screen_size(int width, int height)
  {
    _aspect_ratio = calc_aspect_ratio(width, height);
    ICamera::projection(glm::perspective(
      DEFAULT_FOVY, _aspect_ratio, ICamera::near(), ICamera::far()));
  }
};

}
