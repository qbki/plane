#include <glm/geometric.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/consts.h"

#include "projectiles.h"

void
projectile_handler_system(Scene& scene)
{
  auto& registry = scene.state().shared_registry();
  auto shooter_view = registry->view<Transform,
                                     Lives,
                                     ParticlesEmitter,
                                     DebrisEmitter,
                                     Available>();
  auto
    projectiles_view = registry
                         ->view<Transform, Owner, Available, ProjectileKind>();
  projectiles_view.each(
    [&](entt::entity prj_id, Transform& prj_transform, const Owner& owner) {
      shooter_view.each([&](entt::entity id,
                            const Transform& enemy_position,
                            Lives& lives,
                            const ParticlesEmitter& particles_emitter,
                            const DebrisEmitter& debris_emitter) {
        auto is_owner = owner.value == id;
        if (is_owner) {
          return;
        }
        auto hit_distance = glm::distance(prj_transform.translation(),
                                          enemy_position.translation());
        auto has_hit = hit_distance <= DEFAULT_COLLIDER.radius;
        if (has_hit) {
          lives.value -= 1;
          particles_emitter(prj_transform.translation());
          registry->remove<Available>(prj_id);
          if (lives.value <= 0) {
            lives.value = 0;
            debris_emitter.value(registry, enemy_position.translation());
            scene.handlers().once(
              [id](const Scene& s) { s.state().registry().destroy(id); });
          }
        }
      });
    });
}
