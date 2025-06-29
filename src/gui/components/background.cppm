module;
#include <entt/entt.hpp>
#include <memory>
#include <optional>

export module pln.gui.components.background;

import pln.components.common;
import pln.components.transform;
import pln.events.event;
import pln.events.event_emitter;
import pln.gui.components.rect;
import pln.math.shapes;
import pln.services.app;
import pln.services.events;
import pln.services.theme;
import pln.utils.color;

using namespace pln::components;
using namespace pln::events;

namespace pln::gui::components {

export
struct BackgroundConfig
{
  pln::utils::color::Color color { pln::services::theme().palette.white };
  pln::components::Parent parent { std::nullopt };
  float z { 0 };
};


export
entt::entity
background(std::shared_ptr<entt::registry>& registry,
           const BackgroundConfig& config = {})
{
  auto entity = rect(
    registry,
    {
      .width = static_cast<float>(
        config.parent.value.has_value()
          ? registry->get<pln::math::RectSize>(config.parent.value.value()).width
          : pln::services::app().screen_size().width),
      .height = static_cast<float>(
        config.parent.value.has_value()
          ? registry->get<pln::math::RectSize>(config.parent.value.value()).height
          : pln::services::app().screen_size().height),
      .color = config.color,
      .parent = config.parent,
    });

  auto& layout = registry->emplace<EventEmitter<GUILayout>>(
    entity);

  auto& transform = registry->get<Transform>(entity);
  transform.translate({ 0, 0, config.z });

  layout.add([registry, entity](auto&) {
    auto [rect_size, parent] = registry->get<pln::math::RectSize,
                                             Parent>(entity);
    rect_size = parent.value.has_value()
                  ? registry->get<pln::math::RectSize>(parent.value.value())
                  : pln::services::app().screen_size();
  });

  return entity;
}

}
