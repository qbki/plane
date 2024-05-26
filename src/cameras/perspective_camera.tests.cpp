// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#include <catch2/catch_test_macros.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

#include "src/math/shapes.h"
#include "src/tests-utils/common.h"

#include "perspective_camera.h"

const float NEAR = 0.1;
const float FAR = 1.0;
const float FOVY = glm::radians(90.0);
const int SCREEN_WIDTH = 100;
const int SCREEN_HEIGHT = 100;
const glm::vec3 ORIGIN = glm::zero<glm::vec3>();
const float TERM = glm::normalize(glm::vec2(1, 1)).x;

const Shape::Frustum EXPECTED_FRUSTUM = {
  .near{
    .point = { 0, 0, -NEAR },
    .normal = { 0, 0, -1 },
  },
  .far{
    .point = { 0, 0, -FAR },
    .normal = { 0, 0, 1 },
  },
  .left{
    .point = ORIGIN,
    .normal = { TERM, 0, -TERM },
  },
  .right{
    .point = ORIGIN,
    .normal = { -TERM, 0, -TERM },
  },
  .top{
    .point = ORIGIN,
    .normal = { 0, -TERM, -TERM },
  },
  .bottom{
    .point = ORIGIN,
    .normal = { 0, TERM, -TERM },
  },
};

TEST_CASE("should calculate an expected frustum")
{
  PerspectiveCamera camera(SCREEN_WIDTH, SCREEN_HEIGHT, NEAR, FAR, FOVY);
  auto frustum = camera.frustum();

  check_equality(
    frustum.near.normal, EXPECTED_FRUSTUM.near.normal, "near normal");
  check_equality(frustum.near.point, EXPECTED_FRUSTUM.near.point, "near point");
  check_equality(frustum.far.normal, EXPECTED_FRUSTUM.far.normal, "far normal");
  check_equality(frustum.far.point, EXPECTED_FRUSTUM.far.point, "far point");
  check_equality(
    frustum.left.normal, EXPECTED_FRUSTUM.left.normal, "left normal");
  check_equality(frustum.left.point, EXPECTED_FRUSTUM.left.point, "left point");
  check_equality(
    frustum.right.normal, EXPECTED_FRUSTUM.right.normal, "right normal");
  check_equality(
    frustum.right.point, EXPECTED_FRUSTUM.right.point, "right point");
  check_equality(frustum.top.normal, EXPECTED_FRUSTUM.top.normal, "top normal");
  check_equality(frustum.top.point, EXPECTED_FRUSTUM.top.point, "top point");
  check_equality(
    frustum.bottom.normal, EXPECTED_FRUSTUM.bottom.normal, "bottom normal");
  check_equality(
    frustum.bottom.point, EXPECTED_FRUSTUM.bottom.point, "bottom point");
}

// NOLINTEND(cppcoreguidelines-avoid-do-while)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
