#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <variant>

#include "src/math/shapes.h"

#include "intersection.h"

import pln.shapes;

bool
is_in_front(const Shape::Plane& plane, const Shape::Sphere& sphere)
{
  auto hypotinuse = plane.point - sphere.center;
  if (glm::length(hypotinuse) < sphere.radius) {
    return true;
  }
  auto cos_angle = glm::dot(glm::normalize(hypotinuse), -plane.normal);
  auto adjacent = cos_angle * glm::length(hypotinuse);
  return adjacent > -sphere.radius;
}

bool
is_in_front(const Shape::Plane& plane, const Shape::AABB& aabb)
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

bool
is_in_front(const Shape::Plane& plane, const glm::vec3& point)
{
  auto product = glm::dot(point - plane.point, -plane.normal);
  return product <= 0;
}

bool
are_intersected(const Shape::AABB& a, const Shape::AABB& b)
{
  return (a.min.x <= b.max.x && a.max.x >= b.min.x)
         && (a.min.y <= b.max.y && a.max.y >= b.min.y)
         && (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

struct InFrustumVisitor
{
  const Shape::Frustum* _frustum;

  explicit InFrustumVisitor(const Shape::Frustum* frustum)
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

  bool operator()(const Shape::Sphere& collider) const
  {
    return is_collider_in_front_of_plane(collider);
  }

  bool operator()(const Shape::AABB& collider) const
  {
    return is_collider_in_front_of_plane(collider);
  }

  bool operator()(const Shape::Noop&) const { return true; }

  bool operator()(const Shape::Point& collider) const
  {
    return is_collider_in_front_of_plane(collider.point);
  }
};

bool
is_inside(const Shape::AABB& collider, const glm::vec3& point)
{
  return collider.min.x <= point.x && collider.min.y <= point.y
         && collider.min.z <= point.z && collider.max.x >= point.x
         && collider.max.y >= point.y && collider.max.z >= point.z;
}

bool
is_inside(const pln::shapes::Rect<int>& collider, const glm::ivec2& point)
{
  return collider.x <= point.x && point.x <= (collider.x + collider.width)
         && collider.y <= point.y && point.y <= (collider.y + collider.height);
}

bool
on_axis(const Shape::Sphere& collider, const glm::vec3& point)
{
  return glm::abs(point.x - collider.center.x) <= collider.radius
         || glm::abs(point.y - collider.center.y) <= collider.radius
         || glm::abs(point.z - collider.center.z) <= collider.radius;
}

struct TransformApplyierVisitor
{
  const Transform* _transform;

  explicit TransformApplyierVisitor(const Transform* transform)
    : _transform(transform)
  {
  }

  Shape::Collider operator()(const Shape::Sphere& collider) const
  {
    return Shape::Sphere {
      .center = _transform->translation() + collider.center,
      .radius = collider.radius,
    };
  }

  Shape::Collider operator()(const Shape::AABB& collider) const
  {
    return Shape::AABB {
      .min = _transform->translation() + collider.min,
      .max = _transform->translation() + collider.max,
    };
  }

  Shape::Collider operator()(const Shape::Noop& collider) const
  {
    return collider;
  }

  Shape::Collider operator()(const Shape::Point& collider) const
  {
    return Shape::Point {
      .point = _transform->translation() + collider.point,
    };
  }
};

bool
is_in_frustum(const Shape::Frustum& frustum, const Shape::Collider& collider)
{
  InFrustumVisitor visitor { &frustum };
  return std::visit(visitor, collider);
}

Shape::Collider
apply_transform_to_collider(const Transform& transform,
                            const Shape::Collider& collider)
{
  TransformApplyierVisitor visitor { &transform };
  return std::visit(visitor, collider);
}
