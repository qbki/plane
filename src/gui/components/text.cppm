module;
#include <entt/entt.hpp>
#include <thorvg.h>
#include <utility>

export module pln.gui.components.text;

import pln.components.common;
import pln.components.transform;
import pln.gui.core.theme;
import pln.gui.ui_canvas;
import pln.services.theme;
import pln.shapes;
import pln.utils.color;

using namespace pln::components;
using namespace pln::gui::core;

namespace pln::gui::components {

export
struct TextConfig
{
  pln::utils::color::Color color { pln::services::theme().components.text.color };
  Typography::Font font { pln::services::theme().typography.body1 };
  pln::components::Parent parent { std::nullopt };
  std::string text {""};
};


export
entt::entity
text(std::shared_ptr<entt::registry>& registry, const TextConfig& config = {})
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

  const float twice { 2 };
  float width = bbox_x * twice + bbox_width;
  float height = bbox_y * twice + bbox_height;

  UiCanvas canvas(static_cast<int>(width), static_cast<int>(height));
  canvas.canvas().push(std::move(text));

  Transform transform;
  transform.scale({ width, height, 1 });

  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<pln::utils::color::Color>(entity, config.color);
  registry->emplace<Typography::Font>(entity, config.font);
  registry->emplace<GUIKind>(entity);
  registry->emplace<IsDirty>(entity, true);
  registry->emplace<Parent>(entity, config.parent);
  registry->emplace<pln::shapes::RectSize>(entity, width, height);
  registry->emplace<Text>(entity, config.text);
  registry->emplace<Transform>(entity, transform);
  registry->emplace<UiCanvas>(entity, std::move(canvas));
  return entity;
}

}
