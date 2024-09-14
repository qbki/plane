#include <entt/entt.hpp>
#include <memory>
#include <vector>

namespace GUI {

void
remove_child(std::shared_ptr<entt::registry>& registry,
             entt::entity parent,
             entt::entity child);

void
reparent(std::shared_ptr<entt::registry>& registry,
         const std::vector<entt::entity>& children,
         entt::entity new_parent_entity);

}
