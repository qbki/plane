#pragma once
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <variant>

namespace Shape {

struct Sphere
{
  glm::vec3 center = { 0, 0, 0 };
  float radius = 0;
};

struct AABB
{
  glm::vec3 min = { 0, 0, 0 };
  glm::vec3 max = { 0, 0, 0 };
};

struct Plane
{
  glm::vec3 point = { 0, 0, 0 };
  glm::vec3 normal = { 0, 0, 1 };
};

struct Frustum
{
  Plane near;
  Plane far;
  Plane left;
  Plane right;
  Plane top;
  Plane bottom;
};

struct Point
{
  glm::vec3 point;
};

struct Noop
{};

using Collider = std::variant<Noop, Sphere, AABB, Point>;
}
