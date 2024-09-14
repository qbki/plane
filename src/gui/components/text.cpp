#include <algorithm>
#include <utility>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/gui/core/font.h"
#include "src/gui/types.h"
#include "src/services.h"
#include "src/texture.h"
#include "src/utils/common.h"

#include "text.h"

namespace GUI::Factory {

entt::entity
text(std::shared_ptr<entt::registry>& registry, const TextConfig& config)
{
  auto& font = config.font;
  auto [width, height] = font->calculate_size(config.text);

  auto valid_width = std::max(width, 1);
  auto valid_height = std::max(height, 1);
  auto pixels = get_pixels(valid_width, valid_height);
  auto texture = std::make_unique<Texture>(valid_width, valid_height, pixels);

  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<Core::Color>(entity, config.color);
  registry->emplace<FontPtr>(entity, font);
  registry->emplace<GUIKind>(entity);
  registry->emplace<IsDirty>(entity, true);
  registry->emplace<Parent>(entity, config.parent);
  registry->emplace<RectSize>(entity, width, height);
  registry->emplace<Text>(entity, config.text);
  registry->emplace<Transform>(entity);
  registry->emplace<UniqueTexturePtr>(entity, std::move(texture));

  return entity;
}

}
