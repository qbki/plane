#include <variant>

#include "src/components/common.h"
#include "src/gui/hostiles.h"
#include "src/gui/types.h"

#include "update_gui.h"

void
update_gui(const App::Meta& meta)
{
  auto& registry = meta.app->game_state->registry();

  int enemy_quantity = 0;
  registry.view<const EnemyStateEnum>().each(
    [&enemy_quantity](const EnemyStateEnum& state) {
      if (state == EnemyStateEnum::HUNTING) {
        enemy_quantity++;
      }
    });

  registry.view<GUI::Component>().each([&](GUI::Component& components_sum) {
    std::visit(
      Overloaded{
        [&](GUI::Hostiles& component) { component.quantity(enemy_quantity); },
        [](auto&) {}, // noop
      },
      components_sum);
  });
}
