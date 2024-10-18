#include "src/components/common.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "finish_condition.h"

void
check_finish_condition(Scene& scene)
{
  auto& registry = scene.state().registry();

  int enemy_quantity = 0;
  registry.view<const EnemyStateEnum>().each(
    [&enemy_quantity](const EnemyStateEnum& state) {
      if (state == EnemyStateEnum::HUNTING) {
        enemy_quantity++;
      }
    });

  if (enemy_quantity == 0) {
    Services::app().add_once_handler([](auto&) {
      Services::events<const Events::LoadNextLevelEvent>().emit({});
    });
  }
}
