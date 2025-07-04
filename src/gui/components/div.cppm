module;
#include <algorithm>
#include <entt/entt.hpp>

export module pln.gui.components.div;

import pln.components.common;
import pln.components.transform;
import pln.gui.components.utils;
import pln.math.shapes;

using namespace pln::components;

namespace pln::gui::components {

export
struct DivConfig
{
  float width { 0 };
  float height { 0 };
  pln::components::Parent parent { std::nullopt };
  pln::components::Children children { std::vector<entt::entity>() };
};


export
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
  registry->emplace<pln::math::RectSize>(entity, valid_width, valid_height);
  registry->emplace<Transform>(entity);

  utils::reparent(registry, config.children.value, entity);

  return entity;
}

}
