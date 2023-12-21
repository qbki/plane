#include "./projectiles.h"
#include <entt/entity/fwd.hpp>

void
emit_particles(const App::Meta& meta,
               unsigned int quantity,
               glm::vec3 initial_position)
{
  auto& registry = meta.app->game_state->registry();
  unsigned int idx = 0;
  auto step = glm::two_pi<float>() / static_cast<float>(quantity);
  auto particles = registry.view<Position,
                                 Rotation,
                                 Lifetime,
                                 LifetimeMax,
                                 Velocity,
                                 Speed,
                                 ParticleKind>(entt::exclude<Available>);

  for (auto [id, position, rotation, lifetime, lifetime_max, velocity, speed] :
       particles.each()) {
    position.value = initial_position;
    rotation.value = { 0.0, 0.0, static_cast<float>(idx) * step };
    auto move_direction = calc_z_direction(rotation.value);
    velocity.velocity = move_direction * speed.value;
    registry.emplace<Available>(id);

    lifetime.value = lifetime_max.value;
    idx += 1;
    if (idx >= (quantity - 1)) {
      break;
    }
  }

  for (; idx < (quantity - 1); idx += 1) {
    meta.app->game_state->factory().make_particle(
      registry, initial_position, { 0.0, 0.0, static_cast<float>(idx) * step });
  }
}

void
projectile_handler_system(const App::Meta& meta)
{
  auto& registry = meta.app->game_state->registry();
  auto enemies_view = registry.view<Position,
                                    Velocity,
                                    EnemyStateEnum,
                                    Textures,
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
                          Textures& textures) {
      const float hit_registration_distance = 0.3;
      const float impact_velocity_damping = 0.3;
      const int number_of_new_particles = 20;
      if (glm::distance(prj_position.value, enemy_position.value) <=
          hit_registration_distance) {
        if (enemy_state != EnemyStateEnum::SINKING) {
          enemy_velocity.velocity =
            (glm::vec3(prj_velocity.velocity.x, prj_velocity.velocity.y, 0.0) *
             impact_velocity_damping);
          enemy_state = EnemyStateEnum::SINKING;
          emit_particles(meta, number_of_new_particles, prj_position.value);
          textures.change_type(TextureType::Type::DESTROYED);
          registry.emplace<Gravity>(enemy_id);
        }
        registry.remove<Available>(prj_id);
      }
    });
  });
}
