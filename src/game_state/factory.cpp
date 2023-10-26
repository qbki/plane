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
  auto [mesh, texture] = cache->load("./models/plane.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<PlayerKind>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<TexturePointer>(entity, texture);
  return entity;
}


entt::entity ModelFactory::make_enemy(
  entt::registry& registry,
  glm::vec3 position
) {
  auto [mesh, texture] = cache->load("./models/saucer.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<EnemyKind>(entity);
  registry.emplace<EnemyStateEnum>(entity, EnemyStateEnum::HUNTING);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<TexturePointer>(entity, texture);
  return entity;
}


entt::entity ModelFactory::make_projectile(
  entt::registry& registry,
  glm::vec3 position,
  glm::vec3 rotation
) {
  auto [mesh, texture] = cache->load("./models/bullet.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<InitialPosition>(entity, position);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<ProjectileKind>(entity);
  registry.emplace<Range>(entity, 8.0);
  registry.emplace<Rotation>(entity, rotation);
  registry.emplace<TexturePointer>(entity, texture);
  return entity;
}


entt::entity ModelFactory::make_water_block(
  entt::registry& registry,
  glm::vec3 position
) {
  auto [mesh, texture] = cache->load("./models/water-surface.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<TexturePointer>(entity, texture);
  return entity;
}


entt::entity ModelFactory::make_ground_block(
  entt::registry& registry,
  glm::vec3 position
) {
  auto [mesh, texture] = cache->load("./models/center-block.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, glm::vec3(0.0, 0.0, 0.0));
  registry.emplace<TexturePointer>(entity, texture);
  return entity;
}


entt::entity ModelFactory::make_particle(
  entt::registry& registry,
  glm::vec3 position,
  glm::vec3 rotation
) {
  auto [mesh, texture] = cache->load("./models/particle.glb");
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<Lifetime>(entity, 0.2);
  registry.emplace<LifetimeMax>(entity, 0.2);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<ParticleKind>(entity);
  registry.emplace<Position>(entity, position);
  registry.emplace<Rotation>(entity, rotation);
  registry.emplace<TexturePointer>(entity, texture);
  return entity;
}
