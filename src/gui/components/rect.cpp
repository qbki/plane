#include <entt/entt.hpp>
#include <utility>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/gui/ui_canvas.h"

#include "rect.h"

import pln.utils.color;
import pln.shapes;

namespace GUI::Factory {

entt::entity
rect(std::shared_ptr<entt::registry>& registry, const RectConfig& config)
{
  UiCanvas canvas(static_cast<int>(config.width),
                  static_cast<int>(config.height));

  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<pln::utils::color::Color>(entity, config.color);
  registry->emplace<GUIKind>(entity);
  registry->emplace<IsDirty>(entity, true);
  registry->emplace<Parent>(entity, config.parent);
  registry->emplace<pln::shapes::RectSize>(entity, config.width, config.height);
  registry->emplace<Transform>(entity);
  registry->emplace<UIRect>(entity);
  registry->emplace<UiCanvas>(entity, std::move(canvas));

  return entity;
}

}
