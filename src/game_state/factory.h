#pragma once
#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include <string>

namespace ModelFactory {
using MakerFn = std::function<entt::entity(std::shared_ptr<entt::registry>&,
                                           const std::string&)>;

entt::entity
make_enemy(std::shared_ptr<entt::registry>& registry,
           const std::string& file_path);
entt::entity
make_particle(std::shared_ptr<entt::registry>& registry,
              const std::string& file_path);
entt::entity
make_player(std::shared_ptr<entt::registry>& registry,
            const std::string& file_path);
entt::entity
make_projectile(std::shared_ptr<entt::registry>& registry,
                const std::string& file_path);
entt::entity
make_text(std::shared_ptr<entt::registry>& registry,
          const std::string& file_path);
entt::entity
make_static(std::shared_ptr<entt::registry>& registry,
            const std::string& file_path);
entt::entity
make_debris(std::shared_ptr<entt::registry>& registry,
            const std::string& file_path);

entt::entity
make_directional_light(std::shared_ptr<entt::registry>& registry,
                       const std::string&);
entt::entity
make_point_light(std::shared_ptr<entt::registry>& registry, const std::string&);
entt::entity
make_tutorial_button(std::shared_ptr<entt::registry>& registry,
                     const std::string& file_path);
};
