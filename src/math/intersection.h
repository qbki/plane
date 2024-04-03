#pragma once
#include "shapes.h"
#include "src/components/transform.h"

bool
is_in_front(const Shape::Plane& plane, const Shape::Sphere& sphere);
bool
is_in_front(const Shape::Plane& plane, const Shape::AABB& aabb);
bool
is_in_front(const Shape::Plane& plane, const glm::vec3& point);

bool
is_in_frustum(const Shape::Frustum& frustum, const Shape::Collider& collider);

Shape::Collider
apply_transform_to_collider(const Transform& transform,
                            const Shape::Collider& collider);
