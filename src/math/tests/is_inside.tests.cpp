// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#include <catch2/catch_test_macros.hpp>
#include <glm/vec2.hpp>

#include "../intersection.h"
#include "../shapes.h"

TEST_CASE("is_inside")
{
  Rect<int> rect {
    .x = 0,
    .y = 0,
    .width = 2,
    .height = 2,
  };

  SECTION("should return true")
  {
    REQUIRE(is_inside(rect, { 1, 1 }));
    REQUIRE(is_inside(rect, { 0, 0 }));
  }

  SECTION("should return false")
  {
    REQUIRE_FALSE(is_inside(rect, { -1, 1 }));
    REQUIRE_FALSE(is_inside(rect, { 3, 1 }));
    REQUIRE_FALSE(is_inside(rect, { 1, -1 }));
    REQUIRE_FALSE(is_inside(rect, { 1, 3 }));
  }
}

// NOLINTEND(cppcoreguidelines-avoid-do-while)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
