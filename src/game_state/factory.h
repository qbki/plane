#pragma once
#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "../models/cache.h"

class ModelFactory
{
private:
  std::unique_ptr<Cache> cache;

public:
  using ModelMakerFn =
    std::function<entt::entity(entt::registry&, const std::string&)>;
  using MakerFn = std::function<entt::entity(entt::registry&)>;

  ModelFactory();

  entt::entity make_enemy(entt::registry& registry,
                          const std::string& file_path);
  entt::entity make_particle(entt::registry& registry,
                             const std::string& file_path);
  entt::entity make_player(entt::registry& registry,
                           const std::string& file_path);
  entt::entity make_projectile(entt::registry& registry,
                               const std::string& file_path);
  entt::entity make_static(entt::registry& registry,
                           const std::string& file_path);

  entt::entity make_directional_light(entt::registry& registry);
  entt::entity make_point_light(entt::registry& registry);
};
