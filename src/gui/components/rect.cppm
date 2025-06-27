module;
#include <entt/entt.hpp>
#include <utility>

#include "src/gui/ui_canvas.h"

export module pln.gui.components.rect;

import pln.components.common;
import pln.components.transform;
import pln.services.theme;
import pln.shapes;
import pln.utils.color;

using namespace pln::components;

namespace pln::gui::components {

export
struct RectConfig
{
  float width { 0 };
  float height { 0 };
  pln::utils::color::Color color { pln::services::theme().components.text.color };
  pln::components::Parent parent { std::nullopt };
};


export
entt::entity
rect(std::shared_ptr<entt::registry>& registry,
     const RectConfig& config = {})
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
