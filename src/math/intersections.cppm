module;
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <variant>

export module pln.math.intersections;

import pln.components.transform;
import pln.math.shapes;

namespace pln::math {

export
bool
is_in_front(const Plane& plane, const Sphere& sphere)
{
  auto hypotinuse = plane.point - sphere.center;
  if (glm::length(hypotinuse) < sphere.radius) {
    return true;
  }
  auto cos_angle = glm::dot(glm::normalize(hypotinuse), -plane.normal);
  auto adjacent = cos_angle * glm::length(hypotinuse);
  return adjacent > -sphere.radius;
}


export
bool
is_in_front(const Plane& plane, const glm::vec3& point)
{
  auto product = glm::dot(point - plane.point, -plane.normal);
  return product <= 0;
}


export
bool
is_in_front(const Plane& plane, const AABB& aabb)
{
  // Brute force...
  return is_in_front(plane, aabb.min)
         || is_in_front(plane, { aabb.max.x, aabb.min.y, aabb.min.z })
         || is_in_front(plane, { aabb.max.x, aabb.max.y, aabb.min.z })
         || is_in_front(plane, { aabb.min.x, aabb.max.y, aabb.min.z })
         || is_in_front(plane, aabb.max)
         || is_in_front(plane, { aabb.max.x, aabb.min.y, aabb.max.z })
         || is_in_front(plane, { aabb.min.x, aabb.min.y, aabb.max.z })
         || is_in_front(plane, { aabb.min.x, aabb.max.y, aabb.max.z });
}


export
bool
are_intersected(const AABB& a, const AABB& b)
{
  return (a.min.x <= b.max.x && a.max.x >= b.min.x)
         && (a.min.y <= b.max.y && a.max.y >= b.min.y)
         && (a.min.z <= b.max.z && a.max.z >= b.min.z);
}


export
struct InFrustumVisitor
{
  const Frustum* _frustum;

  explicit InFrustumVisitor(const Frustum* frustum)
    : _frustum(frustum)
  {
  }

  template<typename T>
  [[nodiscard]] bool is_collider_in_front_of_plane(const T& collider) const
  {
    return is_in_front(_frustum->left, collider)
           && is_in_front(_frustum->right, collider)
           && is_in_front(_frustum->top, collider)
           && is_in_front(_frustum->bottom, collider)
           && is_in_front(_frustum->near, collider)
           && is_in_front(_frustum->far, collider);
  }

  bool operator()(const Sphere& collider) const
  {
    return is_collider_in_front_of_plane(collider);
  }

  bool operator()(const AABB& collider) const
  {
    return is_collider_in_front_of_plane(collider);
  }

  bool operator()(const Noop&) const { return true; }

  bool operator()(const Point& collider) const
  {
    return is_collider_in_front_of_plane(collider.point);
  }
};


export
bool
is_inside(const AABB& collider, const glm::vec3& point)
{
  return collider.min.x <= point.x && collider.min.y <= point.y
         && collider.min.z <= point.z && collider.max.x >= point.x
         && collider.max.y >= point.y && collider.max.z >= point.z;
}


export
bool
is_inside(const pln::math::Rect<int>& collider, const glm::ivec2& point)
{
  return collider.x <= point.x && point.x <= (collider.x + collider.width)
         && collider.y <= point.y && point.y <= (collider.y + collider.height);
}


export
bool
on_axis(const Sphere& collider, const glm::vec3& point)
{
  return glm::abs(point.x - collider.center.x) <= collider.radius
         || glm::abs(point.y - collider.center.y) <= collider.radius
         || glm::abs(point.z - collider.center.z) <= collider.radius;
}


export
struct TransformApplyierVisitor
{
  const components::Transform* _transform {nullptr};

  explicit TransformApplyierVisitor(const components::Transform* transform)
    : _transform(transform)
  {
  }

  Collider operator()(const Sphere& collider) const
  {
    return Sphere {
      .center = _transform->translation() + collider.center,
      .radius = collider.radius,
    };
  }

  Collider operator()(const AABB& collider) const
  {
    return AABB {
      .min = _transform->translation() + collider.min,
      .max = _transform->translation() + collider.max,
    };
  }

  Collider operator()(const Noop& collider) const
  {
    return collider;
  }

  Collider operator()(const Point& collider) const
  {
    return Point {
      .point = _transform->translation() + collider.point,
    };
  }
};


export
bool
is_in_frustum(const Frustum& frustum, const Collider& collider)
{
  InFrustumVisitor visitor { &frustum };
  return std::visit(visitor, collider);
}


export
Collider
apply_transform_to_collider(const components::Transform& transform,
                            const Collider& collider)
{
  TransformApplyierVisitor visitor { &transform };
  return std::visit(visitor, collider);
}

}
