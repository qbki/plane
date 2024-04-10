// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#include <catch2/catch_test_macros.hpp>
#include <glm/vec3.hpp>

#include "../intersection.h"
#include "../shapes.h"

TEST_CASE("are_intersected")
{
  SECTION("should return true when AABBs are overlapped")
  {
    Shape::AABB a{ .min = { -2, -2, -2 }, .max = { 1, 1, 1 } };
    Shape::AABB b{ .min = { -1, -1, -1 }, .max = { 2, 2, 2 } };
    REQUIRE(are_intersected(a, b));
    REQUIRE(are_intersected(b, a));
  }

  SECTION("should return true when one AABB is inside another")
  {
    Shape::AABB a{ .min = { -2, -2, -2 }, .max = { 2, 2, 2 } };
    Shape::AABB b{ .min = { -1, -1, -1 }, .max = { 1, 1, 1 } };
    REQUIRE(are_intersected(a, b));
    REQUIRE(are_intersected(b, a));
  }

  SECTION("should return false when AABBs are not overlapped")
  {
    Shape::AABB a{ .min = { 1, 1, 1 }, .max = { 2, 2, 2 } };
    Shape::AABB b{ .min = { -2, -2, -2 }, .max = { -1, -1, -1 } };
    REQUIRE_FALSE(are_intersected(a, b));
    REQUIRE_FALSE(are_intersected(b, a));
  }
}

// NOLINTEND(cppcoreguidelines-avoid-do-while)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
