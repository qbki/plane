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
    auto angle = rotation.value.z;
    auto direction = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0);
    position.value = move_in(position.value, direction, 10.0 * meta.seconds_since_last_frame);
    lifetime.value -= meta.seconds_since_last_frame;
  });
}
