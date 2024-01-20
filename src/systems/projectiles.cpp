#include "src/components/common.h"
#include "src/components/textures.h"
#include "src/components/velocity.h"

#include "projectiles.h"

void
projectile_handler_system(const App::Meta& meta)
{
  auto& registry = meta.app->game_state->registry();
  auto enemies_view = registry.view<Position,
                                    Velocity,
                                    EnemyStateEnum,
                                    Textures,
                                    ParticlesEmitter,
                                    EnemyKind,
                                    Available>();
  auto projectiles_view = registry.view<Position,
                                        InitialPosition,
                                        Range,
                                        Velocity,
                                        Available,
                                        ProjectileKind>();
  projectiles_view.each([&](entt::entity prj_id,
                            Position& prj_position,
                            InitialPosition& prj_initial_position,
                            const Range& prj_range,
                            Velocity& prj_velocity) {
    auto projectile_distance =
      glm::distance(prj_initial_position.value, prj_position.value);

    if (projectile_distance > prj_range.value) {
      registry.remove<Available>(prj_id);
      return;
    }

    enemies_view.each([&](entt::entity enemy_id,
                          Position& enemy_position,
                          Velocity& enemy_velocity,
                          EnemyStateEnum& enemy_state,
                          Textures& textures,
                          const ParticlesEmitter& particles_emitter) {
      const float hit_registration_distance = 0.3;
      const float impact_velocity_damping = 0.3;
      if (glm::distance(prj_position.value, enemy_position.value) <=
          hit_registration_distance) {
        if (enemy_state != EnemyStateEnum::SINKING) {
          enemy_velocity.velocity =
            (glm::vec3(prj_velocity.velocity.x, prj_velocity.velocity.y, 0.0) *
             impact_velocity_damping);
          enemy_state = EnemyStateEnum::SINKING;
          particles_emitter(prj_position.value);
          textures.change_type(TextureType::Type::DESTROYED);
          registry.emplace<Gravity>(enemy_id);
        }
        registry.remove<Available>(prj_id);
      }
    });
  });
}
