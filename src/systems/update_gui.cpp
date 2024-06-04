#include <variant>

#include "src/components/common.h"
#include "src/gui/hostiles.h"
#include "src/gui/types.h"
#include "src/services.h"

#include "update_gui.h"

void
update_gui(Scene& scene)
{
  auto& registry = scene.state().registry();
  registry.view<GUI::Component>().each([&](GUI::Component& components_sum) {
    std::visit(Overloaded{
                 [&](GUI::Hostiles& component) {
                   auto hostiles = app().info().hostiles;
                   component.quantity(hostiles);
                 },
                 [](auto&) {}, // noop
               },
               components_sum);
  });
}

void
update_gui_calculate_hostiles(const Scene& scene)
{
  int enemy_quantity = 0;
  scene.state().registry().view<const EnemyStateEnum>().each(
    [&enemy_quantity](const EnemyStateEnum& state) {
      if (state == EnemyStateEnum::HUNTING) {
        enemy_quantity++;
      }
    });
  app().info().hostiles = enemy_quantity;
}
