#include <algorithm>
#include <entt/entt.hpp>
#include <vector>

#include "src/components/common.h"

#include "utils.h"

namespace GUI {

void
remove_child(std::shared_ptr<entt::registry>& registry,
             entt::entity parent,
             entt::entity child)
{
  auto& parents_children = registry->get<Children>(parent).value;
  auto [rm_begin, rm_end] =
    std::ranges::remove_if(parents_children, [&](const auto& parents_child) {
      return parents_child == child;
    });
  parents_children.erase(rm_begin, rm_end);
}

void
reparent(std::shared_ptr<entt::registry>& registry,
         const std::vector<entt::entity>& children,
         entt::entity new_parent_entity)
{
  for (const auto& child : children) {
    auto& parent = registry->get<Parent>(child);
    if (parent.value.has_value()) {
      remove_child(registry, parent.value.value(), child);
    }
    registry->replace<Parent>(child, Parent{ new_parent_entity });
  }
}

}
