#include <glm/geometric.hpp>

#include "src/components/common.h"
#include "src/components/textures.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"

#include "projectiles.h"

void
projectile_handler_system(const App::Meta& meta)
{
  auto& registry = meta.app->game_state->registry();
  auto enemies_view = registry.view<Transform,
                                    Velocity,
                                    EnemyStateEnum,
                                    Textures,
                                    ParticlesEmitter,
                                    EnemyKind,
                                    Available>();
  auto projectiles_view = registry.view<Transform,
                                        InitialPosition,
                                        Range,
                                        Velocity,
                                        Available,
                                        ProjectileKind>();
  projectiles_view.each([&](entt::entity prj_id,
                            Transform& prj_transform,
                            InitialPosition& prj_initial_position,
                            const Range& prj_range,
                            Velocity& prj_velocity) {
    auto projectile_distance =
      glm::distance(prj_initial_position.value, prj_transform.translation());

    if (projectile_distance > prj_range.value) {
      registry.remove<Available>(prj_id);
      return;
    }

    enemies_view.each([&](entt::entity enemy_id,
                          const Transform& enemy_position,
                          Velocity& enemy_velocity,
                          EnemyStateEnum& enemy_state,
                          Textures& textures,
                          const ParticlesEmitter& particles_emitter) {
      const float hit_registration_distance = 0.3;
      const float impact_velocity_damping = 0.3;
      auto has_hit = glm::distance(prj_transform.translation(),
                                   enemy_position.translation()) <=
                     hit_registration_distance;
      if (has_hit) {
        if (enemy_state != EnemyStateEnum::SINKING) {
          enemy_velocity.velocity =
            (glm::vec3(prj_velocity.velocity.x, prj_velocity.velocity.y, 0.0) *
             impact_velocity_damping);
          enemy_state = EnemyStateEnum::SINKING;
          particles_emitter(prj_transform.translation());
          textures.change_type(TextureType::Type::SECONDARY);
          registry.emplace<Gravity>(enemy_id);
        }
        registry.remove<Available>(prj_id);
      }
    });
  });
}
