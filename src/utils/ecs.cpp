#include <entt/entt.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"

glm::mat4
get_global_matrix(entt::registry& registry, const Parent& parent)
{
  glm::mat4 global_transform = glm::mat4 { 1 };
  auto parent_holder = parent.value;
  while (parent_holder.has_value()) {
    auto [parent_transform,
          new_parent] = registry.get<Transform, Parent>(parent_holder.value());
    global_transform = parent_transform.matrix() * global_transform;
    parent_holder = new_parent.value;
  }
  return global_transform;
}
