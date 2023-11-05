#include "./particles.h"


void particle_handler_system(GameState::Meta& meta) {
  auto& registry = meta.state.registry();
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
