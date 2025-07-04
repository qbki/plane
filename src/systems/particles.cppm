module;
#include <entt/entt.hpp>

export module pln.systems.particles;

import pln.components.common;
import pln.scene.iscene;
import pln.services.app;

using namespace pln::components;

namespace pln::systems::particles {

export
void
particle_handler(pln::scene::IScene& scene)
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

}
