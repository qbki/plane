// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#include <catch2/catch_test_macros.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "src/math/shapes.h"

#include "octree.h"

const Shape::AABB UNIT_BBOX{ .min = { -1, -1, -1 }, .max = { 1, 1, 1 } };

// I have to make a small offset, in other case
// there will be intersections of volumes.
const Shape::AABB BBOX_MIN_MAX_TOP{
  .min = { -1, 0.0001, 0.0001 },
  .max = { -0.0001, 1, 1 },
};
const Shape::AABB BBOX_MAX_MAX_TOP{
  .min = { 0.0001, 0.0001, 0.0001 },
  .max = { 1, 1, 1 },
};
const Shape::AABB BBOX_MIN_MIN_TOP{
  .min = { -1, -1, 0.0001 },
  .max = { -0.0001, -0.0001, 1 },
};
const Shape::AABB BBOX_MAX_MIN_TOP{
  .min = { 0.0001, -1, 0.0001 },
  .max = { 1, -0.0001, 1 },
};
const Shape::AABB BBOX_MIN_MAX_BOTTOM{
  .min = { -1, 0.0001, -1 },
  .max = { -0.0001, 1, -0.0001 },
};
const Shape::AABB BBOX_MAX_MAX_BOTTOM{
  .min = { 0.0001, 0.0001, -1 },
  .max = { 1, 1, -0.0001 },
};
const Shape::AABB BBOX_MIN_MIN_BOTTOM{
  .min = { -1, -1, -1 },
  .max = { -0.0001, -0.0001, -0.0001 },
};
const Shape::AABB BBOX_MAX_MIN_BOTTOM{
  .min = { 0.0001, -1, -1 },
  .max = { 1, -0.0001, -0.0001 },
};

TEST_CASE("Octree")
{
  SECTION(
    "should return an item in a circumscribing valume if that item exists")
  {
    const Shape::Sphere sphere{
      .center = { 0.5, 0.5, 0.5 },
      .radius = 0.4,
    };
    Octree<int> octree(UNIT_BBOX, 1);
    octree.insert(sphere, 42);

    REQUIRE(octree.at(UNIT_BBOX).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MAX_TOP).size() == 0);
    REQUIRE(octree.at(BBOX_MAX_MAX_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MIN_TOP).size() == 0);
    REQUIRE(octree.at(BBOX_MAX_MIN_TOP).size() == 0);
    REQUIRE(octree.at(BBOX_MIN_MAX_BOTTOM).size() == 0);
    REQUIRE(octree.at(BBOX_MAX_MAX_BOTTOM).size() == 0);
    REQUIRE(octree.at(BBOX_MIN_MIN_BOTTOM).size() == 0);
    REQUIRE(octree.at(BBOX_MAX_MIN_BOTTOM).size() == 0);
  }

  SECTION(
    "should return an item in the center of the bounding volume in all cases")
  {
    const Shape::Sphere sphere{
      .center = { 0, 0, 0 },
      .radius = 0.4,
    };
    Octree<int> octree(UNIT_BBOX, 1);
    octree.insert(sphere, 42);

    REQUIRE(octree.at(UNIT_BBOX).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MAX_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MAX_MAX_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MIN_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MAX_MIN_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MAX_BOTTOM).size() == 1);
    REQUIRE(octree.at(BBOX_MAX_MAX_BOTTOM).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MIN_BOTTOM).size() == 1);
    REQUIRE(octree.at(BBOX_MAX_MIN_BOTTOM).size() == 1);
  }

  SECTION("should return an exact item by requesting associated bounding boxes")
  {
    auto sphere = [](float x, float y, float z) {
      return Shape::Sphere{
        .center = { x, y, z },
        .radius = 0.4,
      };
    };
    Octree<int> octree(UNIT_BBOX, 1);
    int min_max_top = 201;
    int max_max_top = 211;
    int min_min_top = 200;
    int max_min_top = 210;
    int min_max_bottom = 101;
    int max_max_bottom = 111;
    int min_min_bottom = 100;
    int max_min_bottom = 110;
    octree.insert(sphere(-0.5, 0.5, 0.5), min_max_top);
    octree.insert(sphere(0.5, 0.5, 0.5), max_max_top);
    octree.insert(sphere(-0.5, -0.5, 0.5), min_min_top);
    octree.insert(sphere(0.5, -0.5, 0.5), max_min_top);
    octree.insert(sphere(-0.5, 0.5, -0.5), min_max_bottom);
    octree.insert(sphere(0.5, 0.5, -0.5), max_max_bottom);
    octree.insert(sphere(-0.5, -0.5, -0.5), min_min_bottom);
    octree.insert(sphere(0.5, -0.5, -0.5), max_min_bottom);

    REQUIRE(octree.at(BBOX_MIN_MAX_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MAX_MAX_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MIN_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MAX_MIN_TOP).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MAX_BOTTOM).size() == 1);
    REQUIRE(octree.at(BBOX_MAX_MAX_BOTTOM).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MIN_BOTTOM).size() == 1);
    REQUIRE(octree.at(BBOX_MAX_MIN_BOTTOM).size() == 1);
    REQUIRE(octree.at(BBOX_MIN_MAX_TOP)[0] == min_max_top);
    REQUIRE(octree.at(BBOX_MAX_MAX_TOP)[0] == max_max_top);
    REQUIRE(octree.at(BBOX_MIN_MIN_TOP)[0] == min_min_top);
    REQUIRE(octree.at(BBOX_MAX_MIN_TOP)[0] == max_min_top);
    REQUIRE(octree.at(BBOX_MIN_MAX_BOTTOM)[0] == min_max_bottom);
    REQUIRE(octree.at(BBOX_MAX_MAX_BOTTOM)[0] == max_max_bottom);
    REQUIRE(octree.at(BBOX_MIN_MIN_BOTTOM)[0] == min_min_bottom);
    REQUIRE(octree.at(BBOX_MAX_MIN_BOTTOM)[0] == max_min_bottom);
  }

  SECTION("should not throw errors on an empty container")
  {
    Octree<int> octree(UNIT_BBOX);
    REQUIRE(octree.at(UNIT_BBOX).size() == 0);
  }
}
// NOLINTEND(cppcoreguidelines-avoid-do-while)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
