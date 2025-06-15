#include "src/components/common.h"
#include "src/scene/scene.h"

#include "finish_condition.h"

import pln.services.app;
import pln.services.events;

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
    pln::services::app().add_once_handler([](auto&) {
        pln::services::events<const Events::LoadNextLevelEvent>().emit({});
    });
  }
}
