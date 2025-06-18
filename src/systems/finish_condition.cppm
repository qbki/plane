module;
#include "src/events/event.h"

export module pln.systems.finish_condition;

import pln.components.common;
import pln.scene.iscene;
import pln.services.app;
import pln.services.events;

using namespace pln::components;

namespace pln::systems::finish_condition {

export
void
check_finish_condition(pln::scene::IScene& scene)
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

}
