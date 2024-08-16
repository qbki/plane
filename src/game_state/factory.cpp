#include <filesystem>
#include <glm/gtc/constants.hpp>

#include "src/components/common.h"
#include "src/components/linear_velocity.h"
#include "src/components/transform.h"
#include "src/services.h"

#include "factory.h"

namespace ModelFactory {

entt::entity
make_player(entt::registry& registry, const std::string& file_path)
{
  auto entity = registry.create();
  auto [mesh, texture] = Services::cache().get_model(file_path);
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<PlayerKind>(entity);
  registry.emplace<ProjectileEmitter>(entity, std::nullopt);
  registry.emplace<TexturePointer>(entity, texture);
  registry.emplace<Transform>(entity);
  registry.emplace<LinearVelocity>(entity);
  return entity;
}

entt::entity
make_enemy(entt::registry& registry, const std::string& file_path)
{
  auto [mesh, texture] = Services::cache().get_model(file_path);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<EnemyKind>(entity);
  registry.emplace<EnemyStateEnum>(entity, EnemyStateEnum::HUNTING);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<TexturePointer>(entity, texture);
  registry.emplace<Transform>(entity);
  registry.emplace<LinearVelocity>(entity);
  return entity;
}

entt::entity
make_debris(entt::registry& registry, const std::string& file_path)
{
  auto [mesh, texture] = Services::cache().get_model(file_path);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<DebrisKind>(entity);
  registry.emplace<Gravity>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<TexturePointer>(entity, texture);
  registry.emplace<Transform>(entity);
  registry.emplace<LinearVelocity>(entity);
  return entity;
}

entt::entity
make_projectile(entt::registry& registry, const std::string& file_path)
{
  auto [mesh, texture] = Services::cache().get_model(file_path);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<InitialPosition>(entity, glm::zero<glm::vec3>());
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<ParticleKind>(entity);
  registry.emplace<ProjectileKind>(entity);
  registry.emplace<Range>(entity, 0);
  registry.emplace<TexturePointer>(entity, texture);
  registry.emplace<Transform>(entity);
  registry.emplace<LinearVelocity>(entity);
  return entity;
}

entt::entity
make_static(entt::registry& registry, const std::string& file_path)
{
  auto [mesh, texture] = Services::cache().get_model(file_path);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<TexturePointer>(entity, texture);
  registry.emplace<Transform>(entity);
  return entity;
}

entt::entity
make_tutorial_button(entt::registry& registry, const std::string& file_path)
{
  auto [mesh, texture] = Services::cache().get_model(file_path);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<Opaque>(entity);
  registry.emplace<TexturePointer>(entity, texture);
  registry.emplace<Transform>(entity);
  registry.emplace<TutorialButton>(entity, Control::Action::UNKNOWN);
  registry.emplace<TutorialButtonKind>(entity);
  return entity;
}

entt::entity
make_particle(entt::registry& registry, const std::string& file_path)
{
  auto [mesh, texture] = Services::cache().get_model(file_path);
  auto entity = registry.create();
  registry.emplace<Available>(entity);
  registry.emplace<Color>(entity, glm::zero<glm::vec3>());
  registry.emplace<Lifetime>(entity, 0);
  registry.emplace<LifetimeMax>(entity, 0);
  registry.emplace<MeshPointer>(entity, mesh);
  registry.emplace<ParticleKind>(entity);
  registry.emplace<TexturePointer>(entity, texture);
  registry.emplace<Transform>(entity);
  registry.emplace<LinearVelocity>(entity);
  return entity;
}

entt::entity
make_point_light(entt::registry& registry, const std::string&)
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
make_directional_light(entt::registry& registry, const std::string&)
{
  auto entity = registry.create();
  registry.emplace<Direction>(entity, glm::zero<glm::vec3>());
  registry.emplace<Color>(entity, glm::zero<glm::vec3>());
  registry.emplace<DirectionalLightKind>(entity);
  return entity;
}

}
