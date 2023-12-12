#include "./particles.h"


void particle_handler_system(App::Meta& meta) {
  auto& registry = meta.app.game_state->registry();
  auto particles = registry.view<
    Position,
    Rotation,
    Lifetime,
    Available,
    ParticleKind
  >();
  particles.each([&](
    entt::entity id,
    Position& position,
    Rotation& rotation,
    Lifetime& lifetime
  ) {
    if (lifetime.value <= 0.0) {
      registry.remove<Available>(id);
      return;
    }
    lifetime.value -= meta.delta_time;
  });
}
