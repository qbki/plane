#include "src/components/common.h"

#include "particles.h"

import pln.services.app;

void
particle_handler_system(Scene& scene)
{
  auto& registry = scene.state().registry();
  auto particles = registry.view<Lifetime, Available, ParticleKind>();
  particles.each([&](entt::entity id, Lifetime& lifetime) {
    if (lifetime.value <= 0.0) {
      registry.remove<Available>(id);
      return;
    }
    lifetime.value -= pln::services::app().delta_time();
  });
}
