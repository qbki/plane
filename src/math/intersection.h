#pragma once
#include "src/components/transform.h"

#include "shapes.h"

bool
is_in_front(const Shape::Plane& plane, const Shape::Sphere& sphere);
bool
is_in_front(const Shape::Plane& plane, const Shape::AABB& aabb);
bool
is_in_front(const Shape::Plane& plane, const glm::vec3& point);

bool
is_in_frustum(const Shape::Frustum& frustum, const Shape::Collider& collider);

bool
is_inside(const Shape::AABB& collider, const glm::vec3& point);

bool
on_axis(const Shape::Sphere& collider, const glm::vec3& point);

bool
are_intersected(const Shape::AABB& a, const Shape::AABB& b);

Shape::Collider
apply_transform_to_collider(const Transform& transform,
                            const Shape::Collider& collider);
