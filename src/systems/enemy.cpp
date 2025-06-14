#define GLM_ENABLE_EXPERIMENTAL
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>
#include <ranges>
#include <tuple>

#include "src/collections/octree.h"
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/turret_rotation.h"
#include "src/components/weapon.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "enemy.h"

constexpr float SHOOT_ANGLE = 0.3;

import pln.consts;
import utils.common;

std::tuple<entt::entity, glm::vec3>
get_player_data(const Scene& scene)
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

void
enemy_hunting_system(Scene& scene)
{
  auto& state = scene.state();
  auto& registry = state.registry();
  auto [player_id, player_position] = get_player_data(scene);
  auto enemies_view = registry
                        .view<Transform,
                              EnemyStateEnum,
                              MeshPointer,
                              Weapon,
                              EnemyKind,
                              Available>()
                        .each();
  auto enemies = std::ranges::subrange(enemies_view)
                 | std::ranges::views::filter([](const auto& tuple) {
                     return std::get<2>(tuple) == EnemyStateEnum::HUNTING;
                   });

  for (auto [id_a, transform_a, _state_a, mesh_a, weapon_a] : enemies) {
    auto position_a = transform_a.translation();
    const auto shooting_distance = weapon_a.bullet_speed * weapon_a.lifetime;
    auto bvolume_a = std::get_if<Shape::Sphere>(&mesh_a->bounding_volume());
    weapon_a.is_shooting = false;
    if (bvolume_a == nullptr) {
      continue;
    }
    auto found_ids = scene.entities()->at(Shape::Sphere {
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
      auto bvolume_b = std::get_if<Shape::Sphere>(&mesh_b->bounding_volume());
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

void
enemy_rotation_system(Scene& scene)
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
          forward_vector, target_vector, Services::app().delta_time());
        enemy_transform.rotate(turret_rotation.quat());
      }
    });
}

void
enemy_initial_rotation(const Scene& scene)
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

void
enemy_acceleration_system(const Scene& scene)
{
  auto [_, player_position] = get_player_data(scene);
  scene.state()
    .shared_registry()
    ->view<Transform, Acceleration, AccelerationScalar, Weapon, EnemyKind>()
    .each([&](const Transform& transform,
              Acceleration& accel,
              const AccelerationScalar& accel_scalar,
              const Weapon& weapon) {
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
