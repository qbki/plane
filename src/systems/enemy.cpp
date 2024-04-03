#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <ranges>
#include <tuple>
#include <variant>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/math/shapes.h"

#include "enemy.h"

const float INACTIVE_ALTITUDE = -2.0;
const float SINKING_ROTATION_SPEED_PER_SEC = glm::two_pi<float>();

bool
are_siblings_close(glm::vec3 a, glm::vec3 b)
{
  const float hit_distance = 0.8;
  return glm::distance(a, b) < hit_distance;
}

void
enemy_hunting_system(const App::Meta& meta)
{
  // TODO Replace by a real AI
  auto player_transform = meta.app->game_state->player<Transform>();
  auto player_position = player_transform.translation();
  auto enemies_view = meta.app->game_state->registry()
                        .view<Transform,
                              Velocity,
                              EnemyStateEnum,
                              MeshPointer,
                              EnemyKind,
                              Available>()
                        .each();
  auto enemies = std::ranges::subrange(enemies_view) |
                 std::ranges::views::filter([](const auto& tuple) {
                   return std::get<3>(tuple) == EnemyStateEnum::HUNTING;
                 });
  for (auto [id_a, transform_a, velocity_a, _state_a, mesh_a] : enemies) {
    glm::vec3 sum{ 0, 0, 0 };
    auto position_a = transform_a.translation();
    auto bvolume_a = std::get_if<Shape::Sphere>(&mesh_a->bounding_volume());
    if (bvolume_a == nullptr) {
      continue;
    }
    for (auto [id_b, transform_b, _velocity_b, _state_b, mesh_b] : enemies) {
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
    velocity_a.acceleration += sum * velocity_a.scalar_acceleration;
  }
}

void
enemy_sinking_system(const App::Meta& meta)
{
  auto& registry = meta.app->game_state->registry();
  meta.app->game_state->registry()
    .view<Transform, EnemyStateEnum, EnemyKind, Available>()
    .each([&](entt::entity id, Transform& transform, EnemyStateEnum& state) {
      if (state == EnemyStateEnum::SINKING) {
        transform.add_rotation_z(SINKING_ROTATION_SPEED_PER_SEC *
                                 meta.delta_time);
        if (transform.translation().z < INACTIVE_ALTITUDE) {
          registry.remove<Available>(id);
          state = EnemyStateEnum::INACTIVE;
        }
      }
    });
}

void
enemy_rotation_system(const App::Meta& meta)
{
  auto& player_transform = meta.app->game_state->player<Transform>();
  meta.app->game_state->registry()
    .view<Transform, EnemyStateEnum, EnemyKind, Available>()
    .each([&player_transform](Transform& enemy_transform,
                              EnemyStateEnum& enemy_state) {
      if (enemy_state == EnemyStateEnum::HUNTING) {
        auto dir_vector =
          player_transform.translation() - enemy_transform.translation();
        enemy_transform.rotate_z(glm::atan(dir_vector.y, dir_vector.x));
      }
    });
}
