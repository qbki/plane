#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/events/event_emitter.h"
#include "src/gui/components/rect.h"
#include "src/shapes.h"

#include "background.h"

import pln.services.app;
import pln.services.events;

namespace GUI::Factory {

entt::entity
background(std::shared_ptr<entt::registry>& registry,
           const BackgroundConfig& config)
{
  auto entity = Factory::rect(
    registry,
    {
      .width = static_cast<float>(
        config.parent.value.has_value()
          ? registry->get<RectSize>(config.parent.value.value()).width
          : pln::services::app().screen_size().width),
      .height = static_cast<float>(
        config.parent.value.has_value()
          ? registry->get<RectSize>(config.parent.value.value()).height
          : pln::services::app().screen_size().height),
      .color = config.color,
      .parent = config.parent,
    });

  auto& layout = registry->emplace<Events::EventEmitter<Events::GUILayout>>(
    entity);

  auto& transform = registry->get<Transform>(entity);
  transform.translate({ 0, 0, config.z });

  layout.add([registry, entity](auto&) {
    auto [rect_size, parent] = registry->get<RectSize, Parent>(entity);
    rect_size = parent.value.has_value()
                  ? registry->get<RectSize>(parent.value.value())
                  : pln::services::app().screen_size();
  });

  return entity;
}

}
