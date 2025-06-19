#include <algorithm>
#include <entt/entt.hpp>
#include <ranges>

#include "row.h"
#include "utils.h"

import pln.components.common;
import pln.components.transform;
import pln.events.event;
import pln.events.event_emitter;
import pln.shapes;

using namespace pln::components;
using namespace pln::events;

namespace GUI::Factory {

entt::entity
row(std::shared_ptr<entt::registry>& registry, const RowConfig& config)
{
  auto entity = registry->create();

  registry->emplace<pln::shapes::RectSize>(entity, 0, 0);
  registry->emplace<Transform>(entity);
  registry->emplace<Parent>(entity, std::nullopt);
  registry->emplace<Children>(entity, config.children);
  auto& layout = registry->emplace<EventEmitter<GUILayout>>(
    entity);

  reparent(registry, config.children.value, entity);

  layout.add([registry, entity](GUILayout &) {
    auto [children_ids,
          transform,
          rect_size,
          parent] = registry
                      ->get<Children, Transform, pln::shapes::RectSize, Parent>(entity);

    auto children = std::views::transform(
      children_ids.value,
      [registry](const auto child) {
        return registry->get<Transform, pln::shapes::RectSize>(child);
      });

    int max_height = 0;
    for (auto [_, child_rect_size] : children) {
      max_height = std::max(max_height, child_rect_size.height);
    }

    pln::shapes::RectSize new_rect_size;
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
