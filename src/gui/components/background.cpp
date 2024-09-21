#include <entt/entt.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/gui/components/rect.h"
#include "src/services.h"

#include "background.h"

namespace GUI::Factory {

entt::entity
background(std::shared_ptr<entt::registry>& registry,
           const BackgroundConfig& config)
{
  auto entity = Factory::rect(registry,
                              {
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
                  : Services::app().screen_size();
  });

  return entity;
}

}
