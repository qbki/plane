#include <entt/entity/fwd.hpp>
#include <iostream>
#include <memory>

#include "../components.h"
#include "factory.h"

ModelFactory::ModelFactory()
  : cache(new Cache()){};

entt::entity
ModelFactory::make_player(entt::registry& registry, glm::vec3 position)
{
  auto [mesh, textures] = cache->load("./models/plane.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<PlayerKind>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<Textures>(entity, textures);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  registry.emplace<Velocity>(entity, 30.0, 0.1);
  return entity;
}

entt::entity
ModelFactory::make_enemy(entt::registry& registry, glm::vec3 position)
{
  auto [mesh, textures] = cache->load("./models/saucer.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<EnemyKind>(entity);
  registry.emplace<EnemyStateEnum>(entity, EnemyStateEnum::HUNTING);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<Textures>(entity, textures);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  registry.emplace<Velocity>(entity, 10.0, 0.1);
  return entity;
}

entt::entity
ModelFactory::make_projectile(entt::registry& registry,
                              glm::vec3 position,
                              glm::vec3 rotation)
{
  auto move_direction = calc_z_direction(rotation);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  auto speed = 30.0f;
  auto [mesh, textures] = cache->load("./models/bullet.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<InitialPosition>(entity, position);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<ProjectileKind>(entity);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  registry.emplace<Range>(entity, 8.0);
  registry.emplace<Rotation>(entity, rotation);
  registry.emplace<Speed>(entity, speed);
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Velocity>(entity, move_direction * speed, 1.0);
  return entity;
}

entt::entity
ModelFactory::make_water_block(entt::registry& registry, glm::vec3 position)
{
  auto [mesh, textures] = cache->load("./models/water-surface.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<Textures>(entity, textures);
  return entity;
}

entt::entity
ModelFactory::make_ground_block(entt::registry& registry, glm::vec3 position)
{
  auto [mesh, textures] = cache->load("./models/center-block.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<Textures>(entity, textures);
  return entity;
}

entt::entity
ModelFactory::make_particle(entt::registry& registry,
                            glm::vec3 position,
                            glm::vec3 rotation)
{
  const auto speed = 15.0f;
  auto move_direction = calc_z_direction(rotation);
  auto [mesh, textures] = cache->load("./models/particle.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  registry.emplace<Lifetime>(entity, 0.4);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  registry.emplace<LifetimeMax>(entity, 0.4);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<ParticleKind>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, rotation);
  registry.emplace<Speed>(entity, speed);
  registry.emplace<Textures>(entity, textures);
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  registry.emplace<Velocity>(entity, move_direction * speed, 0.001);
  return entity;
}

entt::entity
ModelFactory::make_point_light(entt::registry& registry,
                               glm::vec3 position,
                               glm::vec3 color)
{
  PointLightParams point_light_params{
    .constant = 1.0,
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    .linear = 0.045,
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    .quadratic = 0.0075,
  };
  auto entity = registry.create();
  registry.emplace<Position>(entity, position);
  registry.emplace<Color>(entity, color);
  registry.emplace<PointLightParams>(entity, point_light_params);
  registry.emplace<PointLightKind>(entity);
  return entity;
}

entt::entity
ModelFactory::make_directional_light(entt::registry& registry,
                                     glm::vec3 direction,
                                     glm::vec3 color)
{
  auto entity = registry.create();
  registry.emplace<Direction>(entity, direction);
  registry.emplace<Color>(entity, color);
  registry.emplace<DirectionalLightKind>(entity);
  return entity;
}
