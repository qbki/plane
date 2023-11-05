#include <range/v3/view/filter.hpp>

#include "enemy.h"


bool are_siblings_close(glm::vec3 a, glm::vec3 b) {
  return glm::distance(a, b) < 0.8;
}


void enemy_hunting_system(GameState::Meta& meta) {
  // TODO Replace by a real AI
  auto player_pos = meta.state.player<Position>().value;
  auto enemies_view = meta.state.registry().view<
    Position,
    Velocity,
    EnemyStateEnum,
    EnemyKind,
    Available
  >().each();
  auto enemies = ranges::subrange(enemies_view)
    | ranges::views::filter([](const auto& tuple) {
        return std::get<3>(tuple) == EnemyStateEnum::HUNTING;
      });
  for (auto [id_a, pos_a, velocity_a, _state_a] : enemies) {
    glm::vec3 sum {0, 0, 0};
    for (auto [id_b, pos_b, _velocity_b, _state_b] : enemies) {
      if (are_siblings_close(pos_a.value, pos_b.value)) {
        sum += pos_a.value - pos_b.value;
      }
    }
    // found by experiments, it reduces force of attraction to the player and
    // it helps avoiding collapsing enemies during movement
    auto direction_weight = 0.05f;
    sum = glm::normalize(sum + glm::normalize(player_pos - pos_a.value) * direction_weight);
    velocity_a.acceleration += sum * velocity_a.scalar_acceleration;
  }
}


void enemy_sinking_system(GameState::Meta& meta) {
  meta.state.registry().view<
    Position,
    Rotation,
    Velocity,
    EnemyStateEnum,
    EnemyKind,
    Available
  >().each([&](
    entt::entity id,
    Position& position,
    Rotation& rotation,
    Velocity& velocity,
    EnemyStateEnum& state
  ) {
    if (state == EnemyStateEnum::SINKING) {
      rotation.value = {
        0.0,
        0.0,
        rotation.value.z + glm::length(velocity.velocity) * meta.seconds_since_last_frame * 2.0
      };
      if (position.value.z < -2.0) {
        meta.state.registry().remove<Available>(id);
        state = EnemyStateEnum::INACTIVE;
      }
    }
  });
}


void enemy_rotation_system(GameState::Meta& meta) {
  auto& player_positon = meta.state.player<Position>().value;
  meta.state.registry()
    .view<
      Position,
      Rotation,
      EnemyStateEnum,
      EnemyKind,
      Available
    >()
    .each([&player_positon](
      Position& enemy_position,
      Rotation& enemy_rotation,
      EnemyStateEnum& enemy_state
    ) {
      if (enemy_state == EnemyStateEnum::HUNTING) {
        auto dir_vector = player_positon - enemy_position.value;
        enemy_rotation.value = {0.0, 0.0, glm::atan(dir_vector.y, dir_vector.x)};
      }
    });
}
