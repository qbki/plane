#include "src/components/common.h"
#include "src/components/velocity.h"
#include "src/utils/common.h"

#include "emmiters.h"

glm::vec3
calc_spread_angle(float z_rotation)
{
  const auto spread_arc_fraction = 16.0;
  auto max_spread = glm::pi<float>() / spread_arc_fraction;
  const auto half_max_spread = max_spread / 2.0;
  const auto percentage_offset = (std::rand() % 100) * 0.01;
  auto spread = max_spread * percentage_offset;
  return { 0.0, 0.0, z_rotation - half_max_spread + spread };
}

void
emit_particles(App& app,
               glm::vec3 initial_position,
               const ParticleParams& params,
               const std::string& file_path)
{
  auto& registry = app.game_state->registry();
  unsigned int idx = 0;
  auto step = glm::two_pi<float>() / static_cast<float>(params.quantity);
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
    if (idx >= (params.quantity - 1)) {
      break;
    }
  }

  for (; idx < (params.quantity - 1); idx += 1) {
    auto entity =
      app.game_state->factory().make_particle(registry, file_path, 1);
    auto rotation = glm::vec3(0.0, 0.0, static_cast<float>(idx) * step);
    registry.replace<Lifetime>(entity, params.lifetime);
    registry.replace<LifetimeMax>(entity, params.lifetime);
    registry.replace<Position>(entity, initial_position);
    registry.replace<Speed>(entity, params.velocity.speed);
    registry.replace<Rotation>(entity, rotation);
    registry.replace<Velocity>(
      entity,
      Velocity(calc_z_direction(rotation) * params.velocity.speed,
               params.velocity.damping));
  }
}

void
emit_projectile(App& app,
                const ProjectileParams& params,
                const std::string& file_path)
{
  auto& registry = app.game_state->registry();

  auto [player_position, player_rotation] =
    app.game_state->player<Position, Rotation>();
  auto projectiles_view =
    registry.view<ProjectileKind>(entt::exclude<Available>);
  auto rotation = calc_spread_angle(player_rotation.value.z);
  auto projectile_id = projectiles_view.front();
  auto move_direction = calc_z_direction(rotation);
  if (projectile_id == entt::null) {
    auto entity =
      app.game_state->factory().make_projectile(registry, file_path, 1);
    registry.replace<Position>(entity, player_position.value);
    registry.replace<Rotation>(entity, rotation);
    registry.replace<InitialPosition>(entity, player_position.value);
    registry.replace<Speed>(entity, params.velocity.speed);
    registry.replace<Range>(entity, params.range);
    registry.replace<Velocity>(
      entity, move_direction * params.velocity.speed, params.velocity.damping);
  } else {
    auto [prj_position,
          prj_initial_position,
          prj_rotation,
          prj_velocity,
          prj_speed] =
      registry.get<Position, InitialPosition, Rotation, Velocity, Speed>(
        projectile_id);
    prj_position.value = player_position.value;
    prj_initial_position.value = player_position.value;
    prj_rotation.value = rotation;
    prj_velocity.velocity = move_direction * prj_speed.value;
    registry.emplace_or_replace<Available>(projectile_id);
  }
}
