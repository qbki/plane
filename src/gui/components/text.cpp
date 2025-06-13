#include <thorvg.h>
#include <utility>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/gui/ui_canvas.h"
#include "src/services.h"

#include "text.h"

import utils.color;

namespace GUI::Factory {

entt::entity
text(std::shared_ptr<entt::registry>& registry, const TextConfig& config)
{
  auto text = tvg::Text::gen();
  text->font(config.font.name.c_str(), config.font.size);
  text->text(config.text.c_str());
  text->fill(config.color.r, config.color.g, config.color.b);
  text->opacity(config.color.a);

  float bbox_x { 0 };
  float bbox_y { 0 };
  float bbox_width { 0 };
  float bbox_height { 0 };
  text->bounds(&bbox_x, &bbox_y, &bbox_width, &bbox_height, false);

  const float twice = 2;
  float width = bbox_x * twice + bbox_width;
  float height = bbox_y * twice + bbox_height;

  UiCanvas canvas(static_cast<int>(width), static_cast<int>(height));
  canvas.canvas().push(std::move(text));

  Transform transform;
  transform.scale({ width, height, 1 });

  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<utils::color::Color>(entity, config.color);
  registry->emplace<GUI::Typography::Font>(entity, config.font);
  registry->emplace<GUIKind>(entity);
  registry->emplace<IsDirty>(entity, true);
  registry->emplace<Parent>(entity, config.parent);
  registry->emplace<RectSize>(entity, width, height);
  registry->emplace<Text>(entity, config.text);
  registry->emplace<Transform>(entity, transform);
  registry->emplace<UiCanvas>(entity, std::move(canvas));
  return entity;
}

}
