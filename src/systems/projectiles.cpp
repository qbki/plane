#include <glm/geometric.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"

#include "projectiles.h"

void
projectile_handler_system(Scene& scene)
{
  auto& registry = scene.state().registry();
  auto enemies_view =
    registry
      .view<Transform, ParticlesEmitter, DebrisEmitter, EnemyKind, Available>();
  auto projectiles_view =
    registry
      .view<Transform, InitialPosition, Range, Available, ProjectileKind>();
  projectiles_view.each([&](entt::entity prj_id,
                            Transform& prj_transform,
                            InitialPosition& prj_initial_position,
                            const Range& prj_range) {
    auto projectile_distance =
      glm::distance(prj_initial_position.value, prj_transform.translation());

    if (projectile_distance > prj_range.value) {
      registry.remove<Available>(prj_id);
      return;
    }

    enemies_view.each([&](entt::entity enemy_id,
                          const Transform& enemy_position,
                          const ParticlesEmitter& particles_emitter,
                          const DebrisEmitter& debris_emitter) {
      const float hit_registration_distance = 0.3;
      auto hit_distance = glm::distance(prj_transform.translation(),
                                        enemy_position.translation());
      auto has_hit = hit_distance <= hit_registration_distance;
      if (has_hit) {
        particles_emitter(prj_transform.translation());
        debris_emitter.value(registry, enemy_position.translation());
        registry.remove<Available>(prj_id);
        scene.handlers().once([enemy_id](const Scene& s) {
          s.state().registry().destroy(enemy_id);
        });
      }
    });
  });
}
