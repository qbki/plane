#include "./projectiles.h"
#include <entt/entity/fwd.hpp>


void emit_particles(
  GameState::Meta& meta,
  unsigned int quantity,
  glm::vec3 initial_position
) {
  auto& registry = meta.state.registry();
  auto idx = 0;
  auto step = glm::two_pi<float>() / static_cast<float>(quantity);
  auto particles = registry.view<
    Position,
    Rotation,
    Lifetime,
    LifetimeMax,
    Velocity,
    Speed,
    ParticleKind
  >(entt::exclude<Available>);

  for (auto [id, position, rotation, lifetime, lifetime_max, velocity, speed] : particles.each()) {
    position.value = initial_position;
    rotation.value = {0.0, 0.0, idx * step};
    auto move_direction = calc_z_direction(rotation.value);
    velocity.velocity = move_direction * speed.value;
    registry.emplace<Available>(id);

    lifetime.value = lifetime_max.value;
    idx += 1;
    if (idx >= (quantity - 1)) {
      break;
    }
  }

  for(;idx < (quantity - 1); idx += 1) {
    meta.state.factory().make_particle(
      registry,
      initial_position,
      {0.0, 0.0, idx * step}
    );
  }
}


void projectile_handler_system (GameState::Meta& meta) {
  auto& registry = meta.state.registry();
  auto enemies_view = registry.view<
    Position,
    Velocity,
    EnemyStateEnum,
    EnemyKind,
    Available
  >();
  auto projectiles_view = registry.view<
    Position,
    InitialPosition,
    Rotation,
    Range,
    Velocity,
    Available,
    ProjectileKind
  >();
  projectiles_view.each([&](
    entt::entity prj_id,
    Position& prj_position,
    InitialPosition& prj_initial_position,
    Rotation& prj_rotation,
    Range& prj_range,
    Velocity& prj_velocity
  ) {
    auto projectile_distance = glm::distance(
      prj_initial_position.value,
      prj_position.value
    );

    if (projectile_distance > prj_range.value) {
      registry.remove<Available>(prj_id);
      return;
    }

    enemies_view.each([&](
      entt::entity enemy_id,
      Position& enemy_position,
      Velocity& enemy_velocity,
      EnemyStateEnum& enemy_state
    ) {
      if (glm::distance(prj_position.value, enemy_position.value) <= 0.3) {
        if (enemy_state != EnemyStateEnum::SINKING) {
          enemy_velocity.velocity = (
            glm::vec3(prj_velocity.velocity.x, prj_velocity.velocity.y, 0.0) * 0.3f
          );
          enemy_state = EnemyStateEnum::SINKING;
          emit_particles(meta, 20, prj_position.value);
          registry.emplace<Gravity>(enemy_id);
        }
        registry.remove<Available>(prj_id);
      }
    });
  });
}
