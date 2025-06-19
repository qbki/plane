#include <algorithm>
#include <entt/entt.hpp>

#include "div.h"
#include "utils.h"

import pln.components.common;
import pln.components.transform;
import pln.shapes;

using namespace pln::components;

namespace GUI::Factory {

entt::entity
div(std::shared_ptr<entt::registry>& registry, const DivConfig& config)
{
  auto valid_width = std::max(config.width, 0.f);
  auto valid_height = std::max(config.height, 0.f);

  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<Children>(entity, config.children);
  registry->emplace<GUIKind>(entity);
  registry->emplace<Parent>(entity, config.parent);
  registry->emplace<pln::shapes::RectSize>(entity, valid_width, valid_height);
  registry->emplace<Transform>(entity);

  reparent(registry, config.children.value, entity);

  return entity;
}

}
