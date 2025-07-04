module;
#include <entt/entt.hpp>
#include <functional>
#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>

export module pln.game_state.factory;

import pln.components.common;
import pln.components.transform;
import pln.control;
import pln.gui.components.text;
import pln.mesh_generators;
import pln.services.cache;
import pln.services.theme;

using namespace pln::components;

namespace pln::game_state::factory {

export
using MakerFn = std::function<entt::entity(std::shared_ptr<entt::registry>&,
                                           const std::string&)>;


export
entt::entity
make_player(std::shared_ptr<entt::registry>& registry,
            const std::string& file_path)
{

  auto entity = registry->create();
  auto [mesh, texture] = pln::services::cache().get_model(file_path);
  registry->emplace<ActorKind>(entity);
  registry->emplace<Acceleration>(entity, glm::vec3(0, 0, 0));
  registry->emplace<AccelerationScalar>(entity, 0);
  registry->emplace<Available>(entity);
  registry->emplace<MeshPointer>(entity, mesh);
  registry->emplace<PlayerKind>(entity);
  registry->emplace<TexturePointer>(entity, texture);
  registry->emplace<Transform>(entity);
  registry->emplace<Velocity>(entity, glm::vec3(0, 0, 0));
  registry->emplace<VelocityDamping>(entity, 0);
  return entity;
}


export
entt::entity
make_enemy(std::shared_ptr<entt::registry>& registry,
           const std::string& file_path)
{
  auto [mesh, texture] = pln::services::cache().get_model(file_path);
  auto entity = registry->create();
  registry->emplace<ActorKind>(entity);
  registry->emplace<Acceleration>(entity, glm::vec3(0, 0, 0));
  registry->emplace<Available>(entity);
  registry->emplace<EnemyKind>(entity);
  registry->emplace<EnemyStateEnum>(entity, EnemyStateEnum::HUNTING);
  registry->emplace<MeshPointer>(entity, mesh);
  registry->emplace<Opaque>(entity);
  registry->emplace<TexturePointer>(entity, texture);
  registry->emplace<Transform>(entity);
  registry->emplace<Velocity>(entity, glm::vec3(0, 0, 0));
  registry->emplace<VelocityDamping>(entity, 0);
  return entity;
}


export
entt::entity
make_debris(std::shared_ptr<entt::registry>& registry,
            const std::string& file_path)
{
  auto [mesh, texture] = pln::services::cache().get_model(file_path);
  auto entity = registry->create();
  registry->emplace<Acceleration>(entity, glm::vec3(0, 0, 0));
  registry->emplace<Available>(entity);
  registry->emplace<DebrisKind>(entity);
  registry->emplace<Gravity>(entity);
  registry->emplace<MeshPointer>(entity, mesh);
  registry->emplace<Opaque>(entity);
  registry->emplace<TexturePointer>(entity, texture);
  registry->emplace<Transform>(entity);
  registry->emplace<Velocity>(entity, glm::vec3(0, 0, 0));
  return entity;
}


export
entt::entity
make_rect(std::shared_ptr<entt::registry>& registry, const std::string&)
{
  const auto& theme = pln::services::theme();
  auto [mesh, texture] = pln::services::cache().get_rect(theme.palette.WHITE_ACCENT);

  const float size = 0.04;
  Transform transform {};
  transform.scale({ size, size, 1 });

  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<EnemyPointer>(entity);
  registry->emplace<MeshPointer>(entity, mesh);
  registry->emplace<Opaque>(entity);
  registry->emplace<TexturePointer>(entity, texture);
  registry->emplace<Transform>(entity, transform);
  return entity;
}


export
entt::entity
make_projectile(std::shared_ptr<entt::registry>& registry,
                const std::string& file_path)
{
  auto [mesh, texture] = pln::services::cache().get_model(file_path);
  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<Lifetime>(entity, 0);
  registry->emplace<MeshPointer>(entity, mesh);
  registry->emplace<Owner>(entity, static_cast<entt::entity>(entt::null));
  registry->emplace<ParticleKind>(entity);
  registry->emplace<ProjectileKind>(entity);
  registry->emplace<TexturePointer>(entity, texture);
  registry->emplace<Transform>(entity);
  registry->emplace<Velocity>(entity, glm::vec3(0, 0, 0));
  return entity;
}


export
entt::entity
make_static(std::shared_ptr<entt::registry>& registry,
            const std::string& file_path)
{
  auto [mesh, texture] = pln::services::cache().get_model(file_path);
  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<MeshPointer>(entity, mesh);
  registry->emplace<Opaque>(entity);
  registry->emplace<TexturePointer>(entity, texture);
  registry->emplace<Transform>(entity);
  return entity;
}


export
entt::entity
make_text(std::shared_ptr<entt::registry>& registry, const std::string&)
{
  auto entity = pln::gui::components::text(registry);
  registry->emplace<ParticleKind>(entity); // TODO Add better type
  registry->emplace<MeshPointer>(entity, pln::mesh::create_quad());
  return entity;
}


export
entt::entity
make_tutorial_button(std::shared_ptr<entt::registry>& registry,
                     const std::string& file_path)
{
  auto [mesh, texture] = pln::services::cache().get_model(file_path);
  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<MeshPointer>(entity, mesh);
  registry->emplace<Opaque>(entity);
  registry->emplace<TexturePointer>(entity, texture);
  registry->emplace<Transform>(entity);
  registry->emplace<TutorialButton>(entity, pln::control::Action::UNKNOWN);
  registry->emplace<TutorialButtonKind>(entity);
  return entity;
}


export
entt::entity
make_particle(std::shared_ptr<entt::registry>& registry,
              const std::string& file_path)
{
  auto [mesh, texture] = pln::services::cache().get_model(file_path);
  auto entity = registry->create();
  registry->emplace<Available>(entity);
  registry->emplace<Color>(entity, glm::zero<glm::vec3>());
  registry->emplace<Lifetime>(entity, 0);
  registry->emplace<LifetimeMax>(entity, 0);
  registry->emplace<MeshPointer>(entity, mesh);
  registry->emplace<ParticleKind>(entity);
  registry->emplace<TexturePointer>(entity, texture);
  registry->emplace<Transform>(entity);
  registry->emplace<Velocity>(entity, glm::vec3(0, 0, 0));
  return entity;
}


export
entt::entity
make_point_light(std::shared_ptr<entt::registry>& registry, const std::string&)
{
  const PointLightParams point_light_params {
    .constant = 1.0,
    .linear = 0.045,
    .quadratic = 0.0075,
  };
  auto entity = registry->create();
  registry->emplace<Color>(entity, glm::zero<glm::vec3>());
  registry->emplace<PointLightKind>(entity);
  registry->emplace<PointLightParams>(entity, point_light_params);
  registry->emplace<Transform>(entity);
  return entity;
}


export
entt::entity
make_directional_light(std::shared_ptr<entt::registry>& registry,
                       const std::string&)
{
  auto entity = registry->create();
  registry->emplace<Direction>(entity, glm::zero<glm::vec3>());
  registry->emplace<Color>(entity, glm::zero<glm::vec3>());
  registry->emplace<DirectionalLightKind>(entity);
  return entity;
}

}
