#include "src/components/common.h"
#include "src/services.h"

#include "finish_condition.h"

void
check_finish_condition(const App& app)
{
  auto& registry = app.game_state().registry();

  int enemy_quantity = 0;
  registry.view<const EnemyStateEnum>().each(
    [&enemy_quantity](const EnemyStateEnum& state) {
      if (state == EnemyStateEnum::HUNTING) {
        enemy_quantity++;
      }
    });
  if (enemy_quantity == 0) {
    events<Events::WinLevelEvent>().emit();
  }
}
