#define GLM_ENABLE_EXPERIMENTAL
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/trigonometric.hpp>
#include <ranges>
#include <tuple>
#include <variant>

#include "src/collections/octree.h"
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/weapon.h"
#include "src/math/intersection.h"
#include "src/math/shapes.h"
#include "src/scene/scene.h"

#include "enemy.h"

const unsigned int MAX_OCTREE_DEPTH = 12;

void
enemy_hunting_system(Scene& scene)
{
  auto& state = scene.state();
  auto& registry = state.registry();
  Transform player_transform {};
  registry.view<Transform, PlayerKind>().each(
    [&player_transform](const auto& transform) {
      player_transform = transform;
    });
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
  Octree<entt::entity> octree(state.world_bbox(), MAX_OCTREE_DEPTH);
  for (auto [id_a, transform_a, _state_a, mesh_a, weapon_a] : enemies) {
    auto a = apply_transform_to_collider(transform_a,
                                         mesh_a->bounding_volume());
    octree.insert(std::get<Shape::Sphere>(a), id_a);
  }

  for (auto [id_a, transform_a, _state_a, mesh_a, weapon_a] : enemies) {
    auto position_a = transform_a.translation();
    const auto shooting_distance = weapon_a.bullet_speed * weapon_a.lifetime;
    auto bvolume_a = std::get_if<Shape::Sphere>(&mesh_a->bounding_volume());
    weapon_a.is_shooting = false;
    if (bvolume_a == nullptr) {
      continue;
    }
    auto found_ids = octree.at({
      .min = position_a - glm::vec3(shooting_distance),
      .max = position_a + glm::vec3(shooting_distance),
    });

    if (found_ids.empty()) {
      continue;
    }

    auto distance = glm::distance(player_transform.translation(),
                                  transform_a.translation());
    if (distance >= shooting_distance) {
      continue;
    }

    auto direction = glm::normalize(transform_a.rotation()
                                    * glm::vec3(1, 0, 0));
    auto is_shooting = true;

    for (const auto& id_b : found_ids) {
      if (id_a == id_b) {
        continue;
      }
      auto [transform_b, mesh_b] = registry.get<Transform, MeshPointer>(id_b);
      auto bvolume_b = std::get_if<Shape::Sphere>(&mesh_b->bounding_volume());
      float hit_distance = 0.0;
      auto result = glm::intersectRaySphere(transform_a.translation(),
                                            direction,
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
  Transform player_transform {};
  auto& registry = scene.state().registry();
  registry.view<Transform, PlayerKind>().each(
    [&player_transform](const auto& transform) {
      player_transform = transform;
    });
  registry.view<Transform, EnemyStateEnum, EnemyKind, Available>().each(
    [&player_transform](Transform& enemy_transform,
                        EnemyStateEnum& enemy_state) {
      if (enemy_state == EnemyStateEnum::HUNTING) {
        auto dir_vector = player_transform.translation()
                          - enemy_transform.translation();
        enemy_transform.rotate_z(glm::atan(dir_vector.y, dir_vector.x));
      }
    });
}
