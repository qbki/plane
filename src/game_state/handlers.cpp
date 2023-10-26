#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <functional>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <range/v3/all.hpp>
#include <range/v3/view/subrange.hpp>

#include "../components.h"
#include "../utils.h"
#include "factory.h"
#include "game_state.h"
#include "texture_type.h"
#include "handlers.h"


void emit_particles(
  GameState::Meta& meta,
  unsigned int quantity,
  glm::vec3 initial_position
);
bool are_siblings_close(glm::vec3 a, glm::vec3 b);


void move_player(GameState::Meta& meta) {
  glm::vec3 move_normal {0, 0, 0};
  if (meta.control.left) {
    move_normal.x -= 1;
  }
  if (meta.control.right) {
    move_normal.x += 1;
  }
  if (meta.control.up) {
    move_normal.y += 1;
  }
  if (meta.control.down) {
    move_normal.y -= 1;
  }
  auto direction = is_approx_equal(glm::length2(move_normal), 0.0f)
    ? move_normal
    : glm::normalize(move_normal);
  auto& position = meta.state.player<Position>();
  position.value = move_in(
    position.value,
    direction,
    5.0 * meta.seconds_since_last_frame
  );
}


void rotate_player(GameState::Meta& meta) {
  auto [position, rotation] = meta.state.player<Position, Rotation>();
  auto direction = meta.state.cursor() - position.value;
  auto angle = glm::atan(direction.y, direction.x);
  rotation.value = {0.0, 0.0, angle};
}


void shoot_by_player(GameState::Meta& meta) {
  if (!meta.control.is_player_shooting) {
    return;
  }

  auto player_position = meta.state.player<Position>().value;
  auto player_rotation = meta.state.player<Rotation>().value;
  auto projectiles_view = meta.state.registry().view<Position, ProjectileKind>(entt::exclude<Available>);
  auto max_spread = 3.14 / 16.0;
  auto spread = max_spread * (std::rand() % 100) * 0.01;
  auto rotation = glm::vec3(
    0.0,
    0.0,
    player_rotation.z - max_spread / 2.0 + spread
  );
  auto projectile_id = projectiles_view.front();
  if (projectile_id == entt::null) {
    meta.state.factory().make_projectile(meta.state.registry(), player_position, rotation);
  } else {
    meta.state.registry().emplace_or_replace<Position>(projectile_id, player_position);
    meta.state.registry().emplace_or_replace<InitialPosition>(projectile_id, player_position);
    meta.state.registry().emplace_or_replace<Rotation>(projectile_id, rotation);
    meta.state.registry().emplace_or_replace<Available>(projectile_id);
  }
};


void handle_bullets (GameState::Meta& meta) {
  auto enemies_view = meta.state.registry().view<
    Position,
    EnemyStateEnum,
    EnemyKind,
    Available
  >();
  auto projectiles_view = meta.state.registry().view<
    Position,
    InitialPosition,
    Rotation,
    Range,
    Available,
    ProjectileKind
  >();
  projectiles_view.each([&](
    entt::entity projectile_id,
    Position& projectile_position,
    InitialPosition& projectile_initial_position,
    Rotation& projectile_rotation,
    Range& projectile_range
  ) {
    auto projectile_distance = glm::distance(
      projectile_initial_position.value,
      projectile_position.value
    );
    if (projectile_distance > projectile_range.value) {
      meta.state.registry().remove<Available>(projectile_id);
      return;
    }

    auto angle = projectile_rotation.value.z;
    auto direction = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0);
    projectile_position.value = move_in(
      projectile_position.value,
      direction,
      10.0 * meta.seconds_since_last_frame
    );

    enemies_view.each([&](Position& enemy_position, EnemyStateEnum& enemy_state) {
      if (glm::distance(projectile_position.value, enemy_position.value) <= 0.3) {
        if (enemy_state != EnemyStateEnum::SINKING) {
          enemy_state = EnemyStateEnum::SINKING;
          // auto texture_type = TextureType::map_to_int(TextureType::Type::DESTROYED);
          // enemy_state.model->use_basecolor_texture(texture_type);
          emit_particles(meta, 20, projectile_position.value);
        }
        meta.state.registry().remove<Available>(projectile_id);
      }
    });
  });
}


void handle_enemies_hunting(GameState::Meta& meta) {
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


void handle_enemy_sinking(GameState::Meta& meta) {
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


void handle_enemy_rotation(GameState::Meta& meta) {
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


void handle_particles(GameState::Meta& meta) {
  auto& registry = meta.state.registry();
  auto particles = registry.view<
    Position,
    Rotation,
    Lifetime,
    Available,
    ParticleKind
  >();
  particles.each([&](
    entt::entity id,
    Position& position,
    Rotation& rotation,
    Lifetime& lifetime
  ) {
    if (lifetime.value <= 0.0) {
      registry.remove<Available>(id);
      return;
    }
    auto angle = rotation.value.z;
    auto direction = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0);
    position.value = move_in(position.value, direction, 10.0 * meta.seconds_since_last_frame);
    lifetime.value -= meta.seconds_since_last_frame;
  });
}


GameState::Handler make_cursor_handler(int& screen_width, int& screen_height) {
  return ([&screen_width, &screen_height](GameState::Meta meta) {
    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    glm::vec4 viewport {0, 0, screen_width, screen_height};
    auto projection_point = glm::unProject(
      {mouse_x, screen_height - mouse_y, 1.0},
      meta.camera.view(),
      meta.camera.projection(),
      viewport
    );
    auto ray = glm::normalize(projection_point - meta.camera.position());
    float intersection_distance = 0;
    auto [player_position] = meta.state.player<Position>();

    auto has_intersection = glm::intersectRayPlane(
      meta.camera.position(),
      ray,
      {0.0, 0.0, player_position.z},
      {0.0, 0.0, 1.0},
      intersection_distance
    );
    if (has_intersection) {
      meta.state.cursor(meta.camera.position() + ray * intersection_distance);
    }
  });
}


void move_camera(GameState::Meta& meta) {
  auto [player_pos] = meta.state.player<Position>();
  auto camera_pos = meta.state.camera()->position();
  meta.state.camera()->position({player_pos.x, player_pos.y, camera_pos.z});
}


void emit_particles(
  GameState::Meta& meta,
  unsigned int quantity,
  glm::vec3 initial_position
) {
  auto& registry = meta.state.registry();
  auto idx = 0;
  auto step = glm::two_pi<float>() / static_cast<float>(quantity);

  auto particles = registry.view<
    Position,
    Rotation,
    Lifetime,
    LifetimeMax,
    ParticleKind
  >(entt::exclude<Available>);
  for (auto [id, position, rotation, lifetime, lifetime_max] : particles.each()) {
    position.value = initial_position;
    rotation.value = {0.0, 0.0, idx * step};
    registry.emplace<Available>(id);
    lifetime.value = lifetime_max.value;
    idx += 1;
  }

  for(;idx < (quantity - 1); idx += 1) {
    meta.state.factory().make_particle(
      registry,
      initial_position,
      {0.0, 0.0, idx * step}
    );
  }
}


bool are_siblings_close(glm::vec3 a, glm::vec3 b) {
  return glm::distance(a, b) < 0.7;
}
