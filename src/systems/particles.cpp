#include "src/app/app.h"
#include "src/components/common.h"

#include "particles.h"

void
particle_handler_system(const App& app)
{
  auto& registry = app.game_state().registry();
  auto particles = registry.view<Lifetime, Available, ParticleKind>();
  particles.each([&](entt::entity id, Lifetime& lifetime) {
    if (lifetime.value <= 0.0) {
      registry.remove<Available>(id);
      return;
    }
    lifetime.value -= app.delta_time();
  });
}
