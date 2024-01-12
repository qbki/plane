#include "particles.h"
#include "../components.h"

void
particle_handler_system(const App::Meta& meta)
{
  auto& registry = meta.app->game_state->registry();
  auto particles = registry.view<Lifetime, Available, ParticleKind>();
  particles.each([&](entt::entity id, Lifetime& lifetime) {
    if (lifetime.value <= 0.0) {
      registry.remove<Available>(id);
      return;
    }
    lifetime.value -= meta.delta_time;
  });
}
