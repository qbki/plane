#include "src/components/common.h"
#include "src/services.h"

#include "particles.h"

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
    lifetime.value -= Services::app().delta_time();
  });
}
