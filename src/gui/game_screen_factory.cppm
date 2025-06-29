module;
#include <format>
#include <string>
#include <utility>

export module pln.gui.game_screen_factory;

import pln.components.common;
import pln.components.transform;
import pln.gui.components.rect;
import pln.gui.components.text;
import pln.math.shapes;
import pln.scene.iscene;
import pln.services.app;
import pln.services.theme;

using namespace pln::components;

namespace pln::gui {

export
void
game_screen_factory(pln::scene::IScene& scene)
{
  static const int OFFSET = 25;
  static const auto MAX_LIVES_WIDTH = 300;
  static const auto MAX_LIVES_HEIGHT = 20;

  auto screen_size = pln::services::app().screen_size();
  auto& theme = pln::services::theme();
  auto& registry = scene.state().shared_registry();

  auto text = components::text(registry,
                               {
                                 .color = theme.palette.white_accent,
                                 .font = theme.typography.h5,
                                 .text = "Hostiles: 0",
                               });
  auto& text_transform = registry->get<Transform>(text);
  text_transform.translate({ OFFSET, OFFSET, 0 });
  scene.handlers().add([text](pln::scene::IScene& scene) {
    auto hostiles_quantity = pln::services::app().info().hostiles;
    auto& registry = scene.state().registry();
    auto [text_obj, is_dirty] = registry.get<Text, IsDirty>(text);
    auto new_text = std::format("Hostiles: {}", hostiles_quantity);
    if (text_obj.value != new_text) {
      text_obj.value = std::move(new_text);
      is_dirty.value = true;
    }
  });

  auto lives_entity = components::rect(
    registry,
    {
      .width = static_cast<float>(MAX_LIVES_WIDTH),
      .height = static_cast<float>(MAX_LIVES_HEIGHT),
      .color { pln::services::theme().palette.WHITE_ACCENT },
    });
  auto [lives_transform,
        lives_size] = registry->get<Transform, pln::math::RectSize>(lives_entity);
  lives_transform.translate(
    { OFFSET, screen_size.height - lives_size.height - OFFSET, 0 });
  scene.handlers().add([lives_entity](pln::scene::IScene& scene) {
    auto& info = pln::services::app().info();
    auto& registry = scene.state().shared_registry();
    auto size = registry->try_get<pln::math::RectSize>(lives_entity);
    if (size) {
      auto ratio = info.lives.max > 0 ? static_cast<double>(info.lives.value)
                                          / static_cast<double>(info.lives.max)
                                      : 0;
      size->width = static_cast<int>(ratio * MAX_LIVES_WIDTH);
    }
  });
}

}
