#include <entt/entity/fwd.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

#include "src/components/common.h"
#include "src/components/textures.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"

#include "factory.h"

ModelFactory::ModelFactory()
  : cache(new Cache()){};

entt::entity
ModelFactory::make_player(entt::registry& registry,
                          const std::string& file_path,
                          size_t instance_quantity_hint)
{
  auto entity = registry.create();
  auto [mesh, textures] = cache->load(file_path, instance_quantity_hint);
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<PlayerKind>(entity);
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Transform>(entity);
  registry.emplace<Velocity>(entity);
  return entity;
}

entt::entity
ModelFactory::make_enemy(entt::registry& registry,
                         const std::string& file_path,
                         size_t instance_quantity_hint)
{
  auto [mesh, textures] = cache->load(file_path, instance_quantity_hint);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<EnemyKind>(entity);
  registry.emplace<EnemyStateEnum>(entity, EnemyStateEnum::HUNTING);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Transform>(entity);
  registry.emplace<Velocity>(entity);
  return entity;
}

entt::entity
ModelFactory::make_projectile(entt::registry& registry,
                              const std::string& file_path,
                              size_t instance_quantity_hint)
{
  auto [mesh, textures] = cache->load(file_path, instance_quantity_hint);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<InitialPosition>(entity, glm::zero<glm::vec3>());
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<ProjectileKind>(entity);
  registry.emplace<Range>(entity, 0);
  registry.emplace<Speed>(entity, 0);
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Transform>(entity);
  registry.emplace<Velocity>(entity);
  return entity;
}

entt::entity
ModelFactory::make_static(entt::registry& registry,
                          const std::string& file_path,
                          size_t instance_quantity_hint)
{
  auto [mesh, textures] = cache->load(file_path, instance_quantity_hint);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Transform>(entity);
  return entity;
}

entt::entity
ModelFactory::make_particle(entt::registry& registry,
                            const std::string& file_path,
                            size_t instance_quantity_hint)
{
  auto [mesh, textures] = cache->load(file_path, instance_quantity_hint);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<Color>(entity, glm::zero<glm::vec3>());
  registry.emplace<Lifetime>(entity, 0);
  registry.emplace<LifetimeMax>(entity, 0);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<ParticleKind>(entity);
  registry.emplace<Speed>(entity, 0);
  registry.emplace<Textures>(entity, textures);
  registry.emplace<Transform>(entity);
  registry.emplace<Velocity>(entity);
  return entity;
}

entt::entity
ModelFactory::make_point_light(entt::registry& registry,
                               const std::string&,
                               size_t)
{
  const PointLightParams point_light_params{
    .constant = 1.0,
    .linear = 0.045,
    .quadratic = 0.0075,
  };
  auto entity = registry.create();
  registry.emplace<Color>(entity, glm::zero<glm::vec3>());
  registry.emplace<PointLightKind>(entity);
  registry.emplace<PointLightParams>(entity, point_light_params);
  registry.emplace<Transform>(entity);
  return entity;
}

entt::entity
ModelFactory::make_directional_light(entt::registry& registry,
                                     const std::string&,
                                     size_t)
{
  auto entity = registry.create();
  registry.emplace<Direction>(entity, glm::zero<glm::vec3>());
  registry.emplace<Color>(entity, glm::zero<glm::vec3>());
  registry.emplace<DirectionalLightKind>(entity);
  return entity;
}