module;
#include <entt/entt.hpp>
#include <glm/common.hpp>

export module pln.gui.components.block;

import pln.components.common;
import pln.components.transform;
import pln.events.event;
import pln.events.event_emitter;
import pln.gui.components.utils;
import pln.math.shapes;
import pln.services.app;

using namespace pln::components;
using namespace pln::events;

namespace pln::gui::components {

export
struct BlockConfig
{
  pln::components::Children children { std::vector<entt::entity>() };
};


export
entt::entity
block(std::shared_ptr<entt::registry>& registry,
      const BlockConfig& config = {})
{
  auto entity = registry->create();

  registry->emplace<pln::math::RectSize>(entity, 0, 0);
  registry->emplace<Transform>(entity);
  registry->emplace<Parent>(entity, std::nullopt);
  registry->emplace<Children>(entity, config.children);
  auto& layout = registry->emplace<EventEmitter<GUILayout>>(
    entity);

  utils::reparent(registry, config.children.value, entity);

  layout.add([registry, entity](auto&) {
    auto [children,
          transform,
          rect_size,
          parent] = registry
                      ->get<Children, Transform, pln::math::RectSize, Parent>(entity);
    pln::math::RectSize new_rect_size;
    for (auto& child_entity : children.value) {
      auto [child_transform,
            child_rect_size] = registry->get<Transform, pln::math::RectSize>(child_entity);
      auto child_position = child_transform.translation();
      child_transform.translate({ child_position.x,
                                  static_cast<float>(new_rect_size.height),
                                  child_position.z });
      new_rect_size.width = glm::max(new_rect_size.width,
                                     child_rect_size.width);
      new_rect_size.height += child_rect_size.height;
    }
    rect_size = new_rect_size;

    pln::math::RectSize parent_rect_size = parent.value.has_value()
                                  ? registry->get<pln::math::RectSize>(
                                      parent.value.value())
                                  : pln::services::app().screen_size();
    auto left = (parent_rect_size.width - rect_size.width) / 2;
    auto top = (parent_rect_size.height - rect_size.height) / 2;
    auto position = transform.translation();
    transform.translate({ left, top, position.z });
  });

  return entity;
}

}
