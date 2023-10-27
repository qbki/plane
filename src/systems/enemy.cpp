#include <range/v3/view/filter.hpp>

#include "enemy.h"


bool are_siblings_close(glm::vec3 a, glm::vec3 b) {
  return glm::distance(a, b) < 0.7;
}


void enemy_hunting_system(GameState::Meta& meta) {
  // TODO Will be replaced by a real AI
  auto player_pos = meta.state.player<Position>().value;
  auto enemies_view = meta.state.registry().view<
    Position,
    EnemyStateEnum,
    EnemyKind,
    Available
  >().each();
  auto enemies = ranges::subrange(enemies_view)
    | ranges::views::filter([](const auto& tuple) {
        return std::get<2>(tuple) == EnemyStateEnum::HUNTING;
      });
  for (auto [id_a, pos_a, _] : enemies) {
    glm::vec3 sum {0, 0, 0};
    for (auto [id_b, pos_b, _] : enemies) {
      if (are_siblings_close(pos_a.value, pos_b.value)) {
        sum += pos_a.value - pos_b.value;
      }
    }
    // found by experiments, it reduces force of attraction to the player and
    // it helps avoiding collapsing enemies during movement
    auto direction_weight = 0.05f;
    sum = glm::normalize(sum + glm::normalize(player_pos - pos_a.value) * direction_weight);
    pos_a.value = move_in(pos_a.value, sum, meta.seconds_since_last_frame);
  }
}


void enemy_sinking_system(GameState::Meta& meta) {
  meta.state.registry().view<
    Position,
    Rotation,
    EnemyStateEnum,
    EnemyKind,
    Available
  >().each([&](
    entt::entity id,
    Position& position,
    Rotation& rotation,
    EnemyStateEnum& state
  ) {
    if (state == EnemyStateEnum::SINKING) {
      position.value = move_in(position.value, {0, 0, 1}, -1.5 * meta.seconds_since_last_frame);
      rotation.value = {0.0, 0.0, rotation.value.z + 0.05};
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
