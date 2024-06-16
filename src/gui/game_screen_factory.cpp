#include <format>
#include <string>
#include <utility>

#include "src/components/common.h"
#include "src/gui/components/text.h"
#include "src/gui/core/theme.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "game_screen_factory.h"

namespace GUI {

void
game_screen_factory(Scene& scene)
{
  auto& theme = Services::theme();
  auto& registry = scene.state().registry();
  auto text = GUI::Factory::make_text(registry, "Hostiles: 0");
  auto& color = registry.get<Core::Color>(text);
  color = theme.palette.white_accent;
  scene.handlers().add([text](Scene& scene) {
    auto hostiles_quantity = Services::app().info().hostiles;
    auto& registry = scene.state().registry();
    auto [text_obj, is_dirty] = registry.get<Text, IsDirty>(text);
    auto new_text = std::format("Hostiles: {}", hostiles_quantity);
    if (text_obj.value != new_text) {
      text_obj.value = std::move(new_text);
      is_dirty.value = true;
    }
  });
}

}
