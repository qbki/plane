#include <entt/entity/fwd.hpp>
#include <iostream>
#include <memory>

#include "../components.h"
#include "factory.h"


ModelFactory::ModelFactory() : cache(new Cache()) {};


entt::entity ModelFactory::make_player(
  entt::registry& registry,
  glm::vec3 position
) {
  auto [mesh, textures] = cache->load("./models/plane.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<PlayerKind>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Velocity>(entity, 30.0, 0.1);
  return entity;
}


entt::entity ModelFactory::make_enemy(
  entt::registry& registry,
  glm::vec3 position
) {
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
  registry.emplace<Velocity>(entity, 10.0, 0.1);
  return entity;
}


entt::entity ModelFactory::make_projectile(
  entt::registry& registry,
  glm::vec3 position,
  glm::vec3 rotation
) {
  auto move_direction = calc_z_direction(rotation);
  auto speed = 30.0f;
  auto [mesh, textures] = cache->load("./models/bullet.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<InitialPosition>(entity, position);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<ProjectileKind>(entity);
  registry.emplace<Range>(entity, 8.0);
  registry.emplace<Rotation>(entity, rotation);
  registry.emplace<Speed>(entity, speed);
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Velocity>(entity, move_direction * speed, 1.0);
  return entity;
}


entt::entity ModelFactory::make_water_block(
  entt::registry& registry,
  glm::vec3 position
) {
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


entt::entity ModelFactory::make_ground_block(
  entt::registry& registry,
  glm::vec3 position
) {
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


entt::entity ModelFactory::make_particle(
  entt::registry& registry,
  glm::vec3 position,
  glm::vec3 rotation
) {
  auto speed = 15.0f;
  auto move_direction = calc_z_direction(rotation);
  auto [mesh, textures] = cache->load("./models/particle.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<Lifetime>(entity, 0.4);
  registry.emplace<LifetimeMax>(entity, 0.4);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<ParticleKind>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, rotation);
  registry.emplace<Speed>(entity, speed);
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Velocity>(entity, move_direction * speed, 0.001);
  return entity;
}
