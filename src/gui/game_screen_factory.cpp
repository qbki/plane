#include <utility>

#include "src/gui/hostiles.h"
#include "src/gui/types.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "game_screen_factory.h"

void
game_screen_factory(Scene& scene)
{
  auto& registry = scene.state().registry();
  const auto text_block_width = 150;
  const auto text_block_height = 50;
  auto entity = registry.create();
  GUI::Component hostiles =
    GUI::Hostiles(app().theme(), text_block_width, text_block_height);
  registry.emplace<GUI::Component>(entity, std::move(hostiles));
}
