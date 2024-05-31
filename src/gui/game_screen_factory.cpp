#include <utility>

#include "src/gui/hostiles.h"
#include "src/gui/theme.h"
#include "src/gui/types.h"

#include "game_screen_factory.h"

void
game_screen_factory(const App& app)
{
  auto& registry = app.game_state().registry();
  const auto text_block_width = 150;
  const auto text_block_height = 50;
  auto entity = registry.create();
  GUI::Component hostiles =
    GUI::Hostiles(app.theme(), text_block_width, text_block_height);
  registry.emplace<GUI::Component>(entity, std::move(hostiles));
}
