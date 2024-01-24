#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "src/utils/interpolations.h"

using Catch::Matchers::WithinRel;

const float HALF_SMOOTH = 0.5f;

float
half(float x, unsigned int times)
{
  const float delimiter = 2.0;
  if (times == 0) {
    return x;
  }
  return half(x / delimiter, times - 1);
}

TEST_CASE("should return 0.0")
{
  REQUIRE_THAT(damp(0.0, 1.0, 0.0), WithinRel(0.0));
  REQUIRE_THAT(damp(0.0, 1.0, 1.0), WithinRel(0.0));
  REQUIRE_THAT(damp(0.0, 0.0, 0.0), WithinRel(0.0));
  REQUIRE_THAT(damp(0.0, 0.0, 1.0), WithinRel(0.0));
}

TEST_CASE("should return 1.0")
{
  REQUIRE_THAT(damp(1.0, 0.0, 0.0), WithinRel(1.0));
  REQUIRE_THAT(damp(1.0, HALF_SMOOTH, 0.0), WithinRel(1.0));
  REQUIRE_THAT(damp(1.0, 1.0, 0.0), WithinRel(1.0));
}

TEST_CASE("should decrease over time")
{
  const float from = 10.0;
  const float smoothing = HALF_SMOOTH;
  REQUIRE_THAT(damp(from, smoothing, 0), WithinRel(from));
  REQUIRE_THAT(damp(from, smoothing, 1), WithinRel(half(from, 1)));
  REQUIRE_THAT(damp(from, smoothing, 2), WithinRel(half(from, 2)));
}

TEST_CASE("should increase over time")
{
  const float from = -10.0;
  const float smoothing = HALF_SMOOTH;
  REQUIRE_THAT(damp(from, smoothing, 0), WithinRel(from));
  REQUIRE_THAT(damp(from, smoothing, 1), WithinRel(half(from, 1)));
  REQUIRE_THAT(damp(from, smoothing, 2), WithinRel(half(from, 2)));
}
