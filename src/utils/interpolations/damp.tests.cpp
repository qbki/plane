// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "src/utils/interpolations.h"

using Catch::Matchers::WithinRel;

const float HALF_SMOOTH = 0.5f;

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
  REQUIRE_THAT(damp(from, HALF_SMOOTH, 0), WithinRel(from));
  REQUIRE_THAT(damp(from, HALF_SMOOTH, 1), WithinRel(5.0));
  REQUIRE_THAT(damp(from, HALF_SMOOTH, 2), WithinRel(2.5));
}

TEST_CASE("should increase over time")
{
  const float from = -10.0;
  REQUIRE_THAT(damp(from, HALF_SMOOTH, 0), WithinRel(from));
  REQUIRE_THAT(damp(from, HALF_SMOOTH, 1), WithinRel(-5.0));
  REQUIRE_THAT(damp(from, HALF_SMOOTH, 2), WithinRel(-2.5));
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
