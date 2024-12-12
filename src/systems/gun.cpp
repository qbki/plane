#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/weapon.h"
#include "src/consts.h"
#include "src/game_state/factory.h"
#include "src/services.h"
#include "src/utils/random.h"

#include "gun.h"

void
gun_shooting_system(Scene& scene)
{
  auto& registry = scene.state().shared_registry();
  static auto get_random_float = make_random_fn(0.0f, 1.0f);
  registry->view<entt::entity, Weapon, Transform>().each(
    [&](entt::entity owner_entity,
        Weapon& weapon,
        const Transform& owner_transform) {
      weapon.left_time_to_shoot -= Services::app().delta_time();
      if (!weapon.is_shooting || (weapon.left_time_to_shoot > 0.0)) {
        return;
      }
      auto projectile_entity = ModelFactory::make_projectile(
        registry, weapon.bullet_model_path);
      auto spread = weapon.spread * get_random_float() - weapon.spread * HALF;

      ;
      auto direction = owner_transform.rotation()
                       * glm::quat(glm::vec3 { 0, 0, spread })
                       * glm::vec3(1, 0, 0);
      auto xy_direction = glm::normalize(
        glm::vec3(direction.x, direction.y, 0));
      Velocity velocity { xy_direction * weapon.bullet_speed };

      registry->replace<Velocity>(projectile_entity, velocity);
      registry->replace<Owner>(projectile_entity, owner_entity);
      registry->replace<Lifetime>(projectile_entity, weapon.lifetime);

      auto& projectile_transform = registry->get<Transform>(projectile_entity);
      auto owner_euler_rotation = owner_transform.euler() + spread;
      projectile_transform.rotate(glm::vec3(0, 0, owner_euler_rotation.z));
      projectile_transform.translate(owner_transform.translation());

      if (weapon.shot_sound_path.has_value()) {
        Services::events<const Events::ShootEvent>().emit(
          { weapon.shot_sound_path.value() });
      }
      weapon.left_time_to_shoot = 1.0f / weapon.fire_rate;
    });
}
