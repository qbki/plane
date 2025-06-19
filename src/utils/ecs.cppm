module;
#include <entt/entt.hpp>
#include <glm/mat4x4.hpp>

export module pln.utils.ecs;

import pln.components.common;
import pln.components.transform;

using namespace pln::components;

namespace pln::utils::ecs {

export
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

}
