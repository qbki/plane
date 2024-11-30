#include <algorithm>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/services.h"
#include "src/texture.h"

#include "rect.h"

namespace GUI::Factory {

entt::entity
rect(std::shared_ptr<entt::registry>& registry, const RectConfig& config)
{
  auto valid_width = std::max(config.width, 0.f);
  auto valid_height = std::max(config.height, 0.f);

  auto texture = std::make_shared<Texture>(1, 1, config.color.to_array());

  Transform transform;
  transform.scale({ valid_width, valid_height, 1 });

  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<Core::Color>(entity, config.color);
  registry->emplace<GUIKind>(entity);
  registry->emplace<Parent>(entity, config.parent);
  registry->emplace<RectSize>(entity, valid_width, valid_height);
  registry->emplace<Transform>(entity, transform);
  registry->emplace<TexturePointer>(entity, texture);

  return entity;
}

}
