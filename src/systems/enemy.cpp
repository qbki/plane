#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <ranges>
#include <tuple>
#include <variant>

#include "src/collections/octree.h"
#include "src/components/common.h"
#include "src/components/linear_velocity.h"
#include "src/components/transform.h"
#include "src/math/intersection.h"
#include "src/math/shapes.h"
#include "src/scene/scene.h"

#include "enemy.h"

const unsigned int MAX_OCTREE_DEPTH = 12;

bool
are_siblings_close(glm::vec3 a, glm::vec3 b)
{
  const float hit_distance = 0.8;
  return glm::distance(a, b) < hit_distance;
}

void
enemy_hunting_system(Scene& scene)
{
  // TODO Replace by a real AI
  auto& state = scene.state();
  auto& registry = state.registry();
  Transform player_transform{};
  registry.view<Transform, PlayerKind>().each(
    [&player_transform](const auto& transform) {
      player_transform = transform;
    });
  auto player_position = player_transform.translation();
  auto enemies_view = registry
                        .view<Transform,
                              LinearVelocity,
                              EnemyStateEnum,
                              MeshPointer,
                              EnemyKind,
                              Available>()
                        .each();
  auto enemies = std::ranges::subrange(enemies_view) |
                 std::ranges::views::filter([](const auto& tuple) {
                   return std::get<3>(tuple) == EnemyStateEnum::HUNTING;
                 });
  Octree<entt::entity> octree(state.world_bbox(), MAX_OCTREE_DEPTH);
  for (auto [id_a, transform_a, velocity_a, _state_a, mesh_a] : enemies) {
    auto a =
      apply_transform_to_collider(transform_a, mesh_a->bounding_volume());
    octree.insert(std::get<Shape::Sphere>(a), id_a);
  }

  for (auto [id_a, transform_a, velocity_a, _state_a, mesh_a] : enemies) {
    glm::vec3 sum{ 0, 0, 0 };
    auto position_a = transform_a.translation();
    auto bvolume_a = std::get_if<Shape::Sphere>(&mesh_a->bounding_volume());
    if (bvolume_a == nullptr) {
      continue;
    }
    auto found_ids = octree.at({
      .min = transform_a.translation() - glm::vec3(-bvolume_a->radius),
      .max = transform_a.translation() + glm::vec3(bvolume_a->radius),
    });
    for (const auto& id_b : found_ids) {
      auto [transform_b, mesh_b] = registry.get<Transform, MeshPointer>(id_b);
      auto position_b = transform_b.translation();
      auto bvolume_b = std::get_if<Shape::Sphere>(&mesh_b->bounding_volume());
      if (bvolume_b == nullptr) {
        continue;
      }
      auto collapse_distance = bvolume_a->radius + bvolume_b->radius;
      if (glm::distance(position_a, position_b) < collapse_distance) {
        sum += position_a - position_b;
      }
    }
    // found by experiments, it reduces force of attraction to the player and
    // it helps avoiding collapsing enemies during movement
    const auto direction_weight = 0.05f;
    sum = glm::normalize(sum + glm::normalize(player_position - position_a) *
                                 direction_weight);
    velocity_a.velocity = sum * velocity_a.speed;
  }
}

void
enemy_rotation_system(Scene& scene)
{
  Transform player_transform{};
  auto& registry = scene.state().registry();
  registry.view<Transform, PlayerKind>().each(
    [&player_transform](const auto& transform) {
      player_transform = transform;
    });
  registry.view<Transform, EnemyStateEnum, EnemyKind, Available>().each(
    [&player_transform](Transform& enemy_transform,
                        EnemyStateEnum& enemy_state) {
      if (enemy_state == EnemyStateEnum::HUNTING) {
        auto dir_vector =
          player_transform.translation() - enemy_transform.translation();
        enemy_transform.rotate_z(glm::atan(dir_vector.y, dir_vector.x));
      }
    });
}
