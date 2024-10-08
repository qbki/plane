#include <entt/entt.hpp>
#include <ranges>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/services.h"

#include "row.h"
#include "utils.h"

namespace GUI::Factory {

entt::entity
row(std::shared_ptr<entt::registry>& registry, const RowConfig& config)
{
  auto entity = registry->create();

  registry->emplace<RectSize>(entity, 0, 0);
  registry->emplace<Transform>(entity);
  registry->emplace<Parent>(entity, std::nullopt);
  registry->emplace<Children>(entity, config.children);
  auto& layout = registry->emplace<Events::EventEmitter<Events::GUILayout>>(
    entity);

  reparent(registry, config.children.value, entity);

  layout.add([registry, entity](auto&) {
    auto [children_ids,
          transform,
          rect_size,
          parent] = registry
                      ->get<Children, Transform, RectSize, Parent>(entity);

    auto children = children_ids.value
                    | std::views::transform([registry](const auto& child) {
                        return registry->get<Transform, RectSize>(child);
                      });

    int max_height = 0;
    for (auto [_, child_rect_size] : children) {
      max_height = std::max(max_height, child_rect_size.height);
    }

    RectSize new_rect_size;
    new_rect_size.height = max_height;
    for (auto [child_transform, child_rect_size] : children) {
      auto child_position = child_transform.translation();
      child_transform.translate({ static_cast<float>(new_rect_size.width),
                                  (max_height - child_rect_size.height) / 2,
                                  child_position.z });
      new_rect_size.width += child_rect_size.width;
    }
    rect_size = new_rect_size;
  });

  return entity;
}

}
