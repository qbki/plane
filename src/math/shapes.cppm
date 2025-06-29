module;
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <variant>

export module pln.math.shapes;

namespace pln::math {

export
struct RectSize
{
  int width = 0;
  int height = 0;

  RectSize() = default;

  RectSize(int _width, int _height)
    : width(_width)
    , height(_height)
  {
  }
};


export
template<typename T>
struct Rect
{
  T x;
  T y;
  T width;
  T height;
};


export
struct Sphere
{
  glm::vec3 center = { 0, 0, 0 };
  float radius = 0;
};


export
struct AABB
{
  glm::vec3 min = { 0, 0, 0 };
  glm::vec3 max = { 0, 0, 0 };
};


export
struct Plane
{
  glm::vec3 point = { 0, 0, 0 };
  glm::vec3 normal = { 0, 0, 1 };
};


export
struct Frustum
{
  Plane near;
  Plane far;
  Plane left;
  Plane right;
  Plane top;
  Plane bottom;
};


export
struct Point
{
  glm::vec3 point;
};


export
struct Noop
{};


export
using Collider = std::variant<Noop, Sphere, AABB, Point>;

}
