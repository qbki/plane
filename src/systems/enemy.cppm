module;
#define GLM_ENABLE_EXPERIMENTAL
#include <entt/entt.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>
#include <ranges>
#include <tuple>

export module pln.systems.enemy;

import pln.components.common;
import pln.components.transform;
import pln.components.turret_rotation;
import pln.components.weapon;
import pln.consts;
import pln.math.shapes;
import pln.scene.iscene;
import pln.services.app;
import pln.utils.common;

using namespace pln::components;
using namespace pln::math;

namespace pln::systems::enemy {

constexpr float SHOOT_ANGLE = 0.3;


std::tuple<entt::entity, glm::vec3>
get_player_data(const pln::scene::IScene& scene)
{
  entt::entity player_id = entt::null;
  glm::vec3 position {};
  scene.state().shared_registry()->view<Transform, PlayerKind>().each(
    [&](entt::entity entity, const auto& transform) {
      position = transform.translation();
      player_id = entity;
    });
  return { player_id, position };
}

export
void
enemy_hunting(pln::scene::IScene& scene)
{
  auto& state = scene.state();
  auto& registry = state.registry();
  auto [player_id, player_position] = get_player_data(scene);
  auto enemies_view = registry
                        .view<Transform,
                              EnemyStateEnum,
                              MeshPointer,
                              pln::components::Weapon,
                              EnemyKind,
                              Available>()
                        .each();
  for (auto [id_a, transform_a, state_a, mesh_a, weapon_a] : enemies_view) {
    if (state_a != EnemyStateEnum::HUNTING) {
      return;
    }

    auto position_a = transform_a.translation();
    const auto shooting_distance = weapon_a.bullet_speed * weapon_a.lifetime;
    auto bvolume_a = std::get_if<pln::math::Sphere>(&mesh_a->bounding_volume());
    weapon_a.is_shooting = false;
    if (bvolume_a == nullptr) {
      continue;
    }
    auto found_ids = scene.entities()->at(pln::math::Sphere {
      .center = position_a,
      .radius = shooting_distance,
    });

    if (found_ids.empty()) {
      continue;
    }

    auto distance = glm::distance(player_position, transform_a.translation());
    if (distance >= shooting_distance) {
      continue;
    }

    auto forward_direction = transform_a.rotation() * glm::vec3(1, 0, 0);

    // Angle condition
    auto player_direction = glm::normalize(player_position
                                           - transform_a.translation());
    auto angle = glm::acos(glm::dot(player_direction, forward_direction));
    if (angle > SHOOT_ANGLE) {
      continue;
    }

    // Obstacles condition
    auto is_shooting = true;
    for (const auto& id_b : found_ids) {
      if (id_a == id_b || player_id == id_b) {
        continue;
      }
      auto [transform_b, mesh_b] = registry.get<Transform, MeshPointer>(id_b);
      auto bvolume_b = std::get_if<pln::math::Sphere>(&mesh_b->bounding_volume());
      float hit_distance = 0.0;
      auto result = glm::intersectRaySphere(transform_a.translation(),
                                            forward_direction,
                                            transform_b.translation(),
                                            bvolume_b->radius,
                                            hit_distance);
      if (result) {
        is_shooting = false;
        break;
      }
    }
    weapon_a.is_shooting = is_shooting;
  }
}


export
void
enemy_rotation(pln::scene::IScene& scene)
{
  auto& registry = scene.state().registry();
  auto [_, player_position] = get_player_data(scene);
  registry
    .view<Transform, EnemyStateEnum, TurretRotation, EnemyKind, Available>()
    .each([&player_position](Transform& enemy_transform,
                             EnemyStateEnum& enemy_state,
                             TurretRotation& turret_rotation) {
      if (enemy_state == EnemyStateEnum::HUNTING) {
        auto forward_vector = enemy_transform.rotation() * glm::vec3(1, 0, 0);
        auto target_vector = player_position - enemy_transform.translation();
        turret_rotation.rotate(
          forward_vector, target_vector, pln::services::app().delta_time());
        enemy_transform.rotate(turret_rotation.quat());
      }
    });
}


export
void
enemy_initial_rotation(const pln::scene::IScene& scene)
{
  using namespace pln::utils::common;
  auto [_, player_position] = get_player_data(scene);
  scene.state()
    .shared_registry()
    ->view<Transform, TurretRotation, EnemyKind>()
    .each([&player_position](const Transform& enemy_transform,
                             TurretRotation& rotation) {
      if (is_approx_equal(rotation.speed(), 0.0f)) {
        return;
      }
      auto player_direction = glm::normalize(player_position
                                             - enemy_transform.translation());
      auto angle = glm::atan(player_direction.y, player_direction.x);
      rotation.angle(angle);
    });
}


export
void
enemy_acceleration(const pln::scene::IScene& scene)
{
  auto [_, player_position] = get_player_data(scene);
  scene.state()
    .shared_registry()
    ->view<Transform, Acceleration, AccelerationScalar, pln::components::Weapon, EnemyKind>()
    .each([&](const Transform& transform,
              Acceleration& accel,
              const AccelerationScalar& accel_scalar,
              const pln::components::Weapon& weapon) {
      auto max_acceleration_distance = weapon.lifetime * weapon.bullet_speed
                                       * pln::consts::HALF;
      auto raw_direction = transform.rotation() * glm::vec3(1, 0, 0);
      auto direction = glm::vec3(raw_direction.x, raw_direction.y, 0);
      auto distance_to_target = glm::distance(transform.translation(),
                                              player_position);
      if (glm::length2(direction) > 0
          && distance_to_target > max_acceleration_distance) {
        accel.value += glm::normalize(direction) * accel_scalar.value;
      }
    });
}

}
