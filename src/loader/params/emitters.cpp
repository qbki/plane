#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/game_state/factory.h"
#include "src/utils/random.h"

#include "emitters.h"
#include "entities.h"

const auto get_random_percentage = make_random_fn(0.0f, 1.0f);

// This is a placeholder, will be removed after
// implementation of a weapon system.
glm::quat
calc_spread_angle()
{
  const float spread_fraction = 1.0f / 16.0f;
  const float max_spread = glm::pi<float>() * spread_fraction;
  const float half_max_spread = max_spread / 2.0f;
  const float spread = max_spread * get_random_percentage();
  return glm::angleAxis(spread - half_max_spread, glm::vec3(0, 0, 1));
}

glm::quat
calc_rotation(unsigned int multiplier, float angle)
{
  return glm::angleAxis(static_cast<float>(multiplier) * angle,
                        glm::vec3(0, 0, 1));
}

void
emit_particles(const App& app,
               glm::vec3 initial_position,
               const EntityParamsParticles& params,
               const std::string& file_path)
{
  auto& registry = app.game_state().registry();
  unsigned int idx = 0;
  const float step = glm::two_pi<float>() / static_cast<float>(params.quantity);
  auto particles =
    registry
      .view<Transform, Lifetime, LifetimeMax, Velocity, Speed, ParticleKind>(
        entt::exclude<Available>);

  for (auto [id, transform, lifetime, lifetime_max, velocity, speed] :
       particles.each()) {
    auto rotation = calc_rotation(idx, step);
    transform.translate(initial_position).rotate(rotation);
    velocity.velocity = rotation * glm::vec3(1, 0, 0) * speed.value;
    registry.emplace<Available>(id);
    lifetime.value = lifetime_max.value;
    idx += 1;
    if (idx >= (params.quantity - 1)) {
      break;
    }
  }

  for (; idx < params.quantity; idx += 1) {
    auto entity = ModelFactory::make_particle(registry, file_path);
    auto rotation = calc_rotation(idx, step);
    Transform transform;
    transform.translate(initial_position).rotate(rotation);
    registry.replace<Lifetime>(entity, params.lifetime);
    registry.replace<LifetimeMax>(entity, params.lifetime);
    registry.replace<Speed>(entity, params.velocity.speed);
    registry.replace<Transform>(entity, transform);
    registry.replace<Velocity>(
      entity,
      Velocity(rotation * glm::vec3(1, 0, 0) * params.velocity.speed,
               params.velocity.damping));
  }
}

void
emit_projectile(const App& app,
                const EntityParamsGun& params,
                const std::string& file_path)
{
  auto& registry = app.game_state().registry();
  auto projectiles_view =
    registry.view<ProjectileKind>(entt::exclude<Available>);
  auto projectile_id = projectiles_view.front();

  auto& player_transform = app.game_state().player<Transform>();
  auto player_z_rotation =
    glm::angleAxis(player_transform.euler().z, glm::vec3(0, 0, 1));
  auto rotation = player_z_rotation * calc_spread_angle();

  auto move_direction = rotation * glm::vec3(1, 0, 0);

  if (projectile_id == entt::null) {
    auto entity = ModelFactory::make_projectile(registry, file_path);
    Transform transform;
    transform.translate(player_transform.translation()).rotate(rotation);
    registry.replace<Transform>(entity, transform);
    registry.replace<InitialPosition>(entity, player_transform.translation());
    registry.replace<Speed>(entity, params.velocity.speed);
    registry.replace<Range>(entity, params.range);
    registry.replace<Velocity>(
      entity, move_direction * params.velocity.speed, params.velocity.damping);
  } else {
    auto [prj_transform, prj_initial_position, prj_velocity, prj_speed] =
      registry.get<Transform, InitialPosition, Velocity, Speed>(projectile_id);
    prj_transform.translate(player_transform.translation()).rotate(rotation);
    prj_initial_position.value = player_transform.translation();
    prj_velocity.velocity = move_direction * prj_speed.value;
    registry.emplace_or_replace<Available>(projectile_id);
  }
}
