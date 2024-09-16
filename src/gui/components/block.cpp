#include <entt/entt.hpp>
#include <glm/common.hpp>
#include <ranges>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/services.h"

#include "block.h"
#include "utils.h"

namespace GUI::Factory {

entt::entity
block(std::shared_ptr<entt::registry>& registry, const BlockConfig& config)
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
    auto [children,
          transform,
          rect_size,
          parent] = registry
                      ->get<Children, Transform, RectSize, Parent>(entity);
    RectSize new_rect_size;
    for (auto& child_entity : std::views::reverse(children.value)) {
      auto [child_transform,
            child_rect_size] = registry->get<Transform, RectSize>(child_entity);
      auto child_position = child_transform.translation();
      child_transform.translate({ child_position.x,
                                  static_cast<float>(new_rect_size.height),
                                  child_position.z });
      new_rect_size.width = glm::max(new_rect_size.width,
                                     child_rect_size.width);
      new_rect_size.height += child_rect_size.height;
    }
    rect_size = new_rect_size;

    RectSize parent_rect_size = parent.value.has_value()
                                  ? registry->get<RectSize>(
                                      parent.value.value())
                                  : Services::app().screen_size();
    auto left = (parent_rect_size.width - rect_size.width) / 2;
    auto bottom = (parent_rect_size.height - rect_size.height) / 2;
    auto position = transform.translation();
    transform.translate({ left, bottom, position.z });
  });

  return entity;
}

}
