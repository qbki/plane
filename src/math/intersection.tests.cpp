// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include "intersection.h"
#include "src/math/shapes.h"

const Shape::Plane PLANE{
  .point{ 0, 0, 0 },
  .normal{ 0, 1, 0 },
};

Shape::Sphere
sphere(glm::vec3 center)
{
  return {
    .center = center,
    .radius = 0.5,
  };
}

Shape::AABB
aabb(glm::vec3 center)
{
  return { .min = center - glm::vec3(0.5, 0.5, 0.5),
           .max = center + glm::vec3(0.5, 0.5, 0.5) };
}

TEST_CASE("Sphere collider")
{
  SECTION("should return true when spheres are in front of a plane")
  {
    REQUIRE(is_in_front(PLANE, sphere({ 1, 1, -1 })));
    REQUIRE(is_in_front(PLANE, sphere({ 0, 1, 0 })));
    REQUIRE(is_in_front(PLANE, sphere({ -1, 1, 1 })));
  }
  SECTION("should return true when spheres are on a plane")
  {
    REQUIRE(is_in_front(PLANE, sphere({ 1, 0, -1 })));
    REQUIRE(is_in_front(PLANE, sphere({ 0, 0, 0 })));
    REQUIRE(is_in_front(PLANE, sphere({ -1, 0, 1 })));
  }
  SECTION("should return false when spheres are behind a plane")
  {
    REQUIRE_FALSE(is_in_front(PLANE, sphere({ 1, -1, -1 })));
    REQUIRE_FALSE(is_in_front(PLANE, sphere({ 0, -1, 0 })));
    REQUIRE_FALSE(is_in_front(PLANE, sphere({ -1, -1, 1 })));
  }
}

TEST_CASE("Point collider")
{
  SECTION("should return true when points are in front of a plane")
  {
    REQUIRE(is_in_front(PLANE, glm::vec3(1, 1, -1)));
    REQUIRE(is_in_front(PLANE, glm::vec3(0, 1, 0)));
    REQUIRE(is_in_front(PLANE, glm::vec3(-1, 1, 1)));
  }
  SECTION("should return true when points are on a plane")
  {
    REQUIRE(is_in_front(PLANE, glm::vec3(1, 0, -1)));
    REQUIRE(is_in_front(PLANE, glm::vec3(0, 0, 0)));
    REQUIRE(is_in_front(PLANE, glm::vec3(-1, 0, 1)));
  }
  SECTION("should return false when points are behind a plane")
  {
    REQUIRE_FALSE(is_in_front(PLANE, glm::vec3(1, -1, -1)));
    REQUIRE_FALSE(is_in_front(PLANE, glm::vec3(0, -1, 0)));
    REQUIRE_FALSE(is_in_front(PLANE, glm::vec3(-1, -1, 1)));
  }
}

TEST_CASE("AABB collider")
{
  SECTION("should return true when AABBs are in front of a plane")
  {
    REQUIRE(is_in_front(PLANE, aabb({ 1, 1, -1 })));
    REQUIRE(is_in_front(PLANE, aabb({ 0, 1, 0 })));
    REQUIRE(is_in_front(PLANE, aabb({ -1, 1, 1 })));
  }

  SECTION("should return true when AABBs are on a plane")
  {
    REQUIRE(is_in_front(PLANE, aabb({ 1, 0, -1 })));
    REQUIRE(is_in_front(PLANE, aabb({ 0, 0, 0 })));
    REQUIRE(is_in_front(PLANE, aabb({ -1, 0, 1 })));
  }

  SECTION("should return false when AABBs are behind a plane")
  {
    REQUIRE_FALSE(is_in_front(PLANE, aabb({ 1, -1, -1 })));
    REQUIRE_FALSE(is_in_front(PLANE, aabb({ 0, -1, 0 })));
    REQUIRE_FALSE(is_in_front(PLANE, aabb({ -1, -1, 1 })));
  }
}

// NOLINTEND(cppcoreguidelines-avoid-do-while)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
