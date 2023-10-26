#pragma once
#include <memory>
#include <entt/entt.hpp>

#include "../models/index.h"

class ModelFactory {
private:
  std::unique_ptr<Cache> cache;

public:
  ModelFactory();
  entt::entity make_player(entt::registry& registry, glm::vec3 position);
  entt::entity make_water_block(entt::registry& registry, glm::vec3 position);
  entt::entity make_ground_block(entt::registry& registry, glm::vec3 position);
  entt::entity make_enemy(entt::registry& registry, glm::vec3 position);
  entt::entity make_particle(
    entt::registry& registry,
    glm::vec3 position,
    glm::vec3 rotation
  );
  entt::entity make_projectile(
    entt::registry& registry,
    glm::vec3 position,
    glm::vec3 rotation
  );
};
