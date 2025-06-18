module;
#include <cstdlib>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/events/event.h"
#include "src/game_state/factory.h"
#include "src/utils/random.h"

export module pln.systems.gun;

import pln.components.weapon;
import pln.consts;
import pln.scene.iscene;
import pln.services.app;
import pln.services.events;
import pln.utils.common;

namespace pln::systems::gun {

export
void
gun_shooting(pln::scene::IScene& scene)
{
  using namespace pln::utils::common;

  auto& registry = scene.state().shared_registry();
  static auto get_random_float = make_random_fn(0.0f, 1.0f);
  registry->view<entt::entity,
                 pln::components::Weapon,
                 Transform>().each(
    [&](entt::entity owner_entity,
        pln::components::Weapon& weapon,
        const Transform& owner_transform) {
      weapon.left_time_to_shoot -= pln::services::app().delta_time();
      if (!weapon.is_shooting || (weapon.left_time_to_shoot > 0.0)) {
        return;
      }
      auto projectile_entity = ModelFactory::make_projectile(
        registry, weapon.bullet_model_path);
      auto spread = weapon.spread * get_random_float() - weapon.spread * pln::consts::HALF;

      auto direction = owner_transform.rotation()
                       * glm::quat(glm::vec3 { 0, 0, spread })
                       * glm::vec3(1, 0, 0);
      auto xy_direction = glm::normalize(
        glm::vec3(direction.x, direction.y, 0));

      auto owner_velocity_wrapper = registry->try_get<Velocity>(owner_entity);
      auto owner_velocity = owner_velocity_wrapper == nullptr
                              ? glm::vec3(0, 0, 0)
                              : owner_velocity_wrapper->value;

      Velocity velocity { owner_velocity + xy_direction * weapon.bullet_speed };

      registry->replace<Velocity>(projectile_entity, velocity);
      registry->replace<Owner>(projectile_entity, owner_entity);
      registry->replace<Lifetime>(projectile_entity, weapon.lifetime);

      auto& projectile_transform = registry->get<Transform>(projectile_entity);
      auto owner_euler_rotation = owner_transform.euler() + spread;
      projectile_transform.rotate(glm::vec3(0, 0, owner_euler_rotation.z));
      projectile_transform.translate(owner_transform.translation());

      if (weapon.shot_sound_path.has_value()) {
        auto player_position = pln::services::app().info().player_position;
        auto owner_position = owner_transform.translation();
        auto distance = glm::distance(owner_position, player_position);

        // <>
        // Used modified Inverse Square Law for Sound.
        // The coefficient was lined up empirically.
        const auto coefficient = 10.0f;
        auto intencity = is_approx_equal(distance, 0.0f)
                           ? 1.0f
                           : coefficient / (distance * distance);
        // </>

        pln::services::events<const Events::ShootEvent>().emit({
          .sound_path { weapon.shot_sound_path.value() },
          .volume = intencity,
        });
      }
      weapon.left_time_to_shoot = 1.0f / weapon.fire_rate;
    });
}

}
