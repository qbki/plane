#pragma once
#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "src/models/cache.h"

class ModelFactory
{
private:
  std::unique_ptr<Cache> cache;

public:
  using MakerFn =
    std::function<entt::entity(entt::registry&, const std::string&)>;

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

  entt::entity make_directional_light(entt::registry& registry,
                                      const std::string&);
  entt::entity make_point_light(entt::registry& registry, const std::string&);
  entt::entity make_tutorial_button(entt::registry& registry,
                                    const std::string& file_path);
};
