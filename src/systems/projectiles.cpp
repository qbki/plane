#include "./projectiles.h"


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
    ParticleKind
  >(entt::exclude<Available>);
  for (auto [id, position, rotation, lifetime, lifetime_max] : particles.each()) {
    position.value = initial_position;
    rotation.value = {0.0, 0.0, idx * step};
    registry.emplace<Available>(id);
    lifetime.value = lifetime_max.value;
    idx += 1;
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
  auto enemies_view = meta.state.registry().view<
    Position,
    EnemyStateEnum,
    EnemyKind,
    Available
  >();
  auto projectiles_view = meta.state.registry().view<
    Position,
    InitialPosition,
    Rotation,
    Range,
    Available,
    ProjectileKind
  >();
  projectiles_view.each([&](
    entt::entity projectile_id,
    Position& projectile_position,
    InitialPosition& projectile_initial_position,
    Rotation& projectile_rotation,
    Range& projectile_range
  ) {
    auto projectile_distance = glm::distance(
      projectile_initial_position.value,
      projectile_position.value
    );
    if (projectile_distance > projectile_range.value) {
      meta.state.registry().remove<Available>(projectile_id);
      return;
    }

    auto angle = projectile_rotation.value.z;
    auto direction = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0);
    projectile_position.value = move_in(
      projectile_position.value,
      direction,
      10.0 * meta.seconds_since_last_frame
    );

    enemies_view.each([&](Position& enemy_position, EnemyStateEnum& enemy_state) {
      if (glm::distance(projectile_position.value, enemy_position.value) <= 0.3) {
        if (enemy_state != EnemyStateEnum::SINKING) {
          enemy_state = EnemyStateEnum::SINKING;
          // auto texture_type = TextureType::map_to_int(TextureType::Type::DESTROYED);
          // enemy_state.model->use_basecolor_texture(texture_type);
          emit_particles(meta, 20, projectile_position.value);
        }
        meta.state.registry().remove<Available>(projectile_id);
      }
    });
  });
}
