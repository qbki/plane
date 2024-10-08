#include <algorithm>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/services.h"

#include "div.h"
#include "utils.h"

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
  registry->emplace<RectSize>(entity, valid_width, valid_height);
  registry->emplace<Transform>(entity);

  reparent(registry, config.children.value, entity);

  return entity;
}

}
