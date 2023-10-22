#include <cstdlib>
#include <functional>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <vector>

#include "game_state.h"
#include "texture_type.h"


template<typename T>
std::vector<T*> filter_ptr(std::vector<T>& items, std::function<bool(T&)> predicate) {
  std::vector<EnemyState*> filtered_items;
  for (auto& item : items) {
    if (predicate(item)) {
      filtered_items.push_back(&item);
    }
  }
  return filtered_items;
}


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
  meta.state.player()->move_in(direction, 5.0 * meta.seconds_since_last_frame);
}


void rotate_player(GameState::Meta& meta) {
  auto player = meta.state.player();
  auto direction = meta.state.cursor() - player->position();
  auto angle = glm::atan(direction.y, direction.x);
  player->rotation_z(angle);
}


void shoot_by_player(GameState::Meta& meta) {
  const auto& player = meta.state.player();
  if (meta.control.is_player_shooting) {
    for (auto& projectile : meta.state.projectiles()) {
      auto model = projectile.model();
      if (!model->is_active()) {
        auto start_position = player->position();
        model->is_active(true);
        model->position(start_position);
        // TEMP: It will be moved into Weapon class in the future
        auto max_spread = 3.14 / 16.0;
        auto spread = max_spread * (std::rand() % 100) * 0.01;
        model->rotation_z(player->rotation_z() - max_spread / 2.0 + spread);
        projectile.start_point(start_position);
        break;
      }
    }
  }
};


void handle_bullets (GameState::Meta& meta) {
  for (auto& projectile : meta.state.projectiles()) {
    auto projectile_model = projectile.model();
    if (!projectile_model->is_active()) {
      continue;
    }

    auto angle = projectile_model->rotation_z();
    auto direction = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0);
    projectile_model->move_in(direction, 10.0 * meta.seconds_since_last_frame);

    auto projectile_distance = glm::distance(
        projectile.start_point(),
        projectile_model->position());
    if (projectile_distance > projectile.distance()) {
      projectile_model->is_active(false);
      continue;
    }

    for (auto& enemy_state : meta.state.enemies_state()) {
      auto enemy = enemy_state.model;
      if (glm::distance(projectile_model->position(), enemy->position()) <= 0.3) {
        if (enemy_state.state != EnemyState::SINKING) {
          enemy_state.state = EnemyState::SINKING;
          auto texture_type = TextureType::map_to_int(TextureType::Type::DESTROYED);
          enemy_state.model->use_basecolor_texture(texture_type);
          meta.state.particle_emitter().emit(
              20,
              projectile_model->position(),
              {0.0, 0.0, 1.0});
        }
        projectile_model->is_active(false);
      }
    }
  }
}


void handle_enemies_hunting(GameState::Meta& meta) {
  auto player = meta.state.player();
  auto filtered_enemies_state = filter_ptr<EnemyState>(
    meta.state.enemies_state(),
    [](EnemyState& v) { return v.state == EnemyState::HUNTING; }
  );
  for (auto enemy_state : filtered_enemies_state) {
    auto enemy {enemy_state->model.get()};
    auto pos_a {enemy->position()};
    glm::vec3 sum {0, 0, 0};
    for (auto near_enemy_state : filtered_enemies_state) {
      if (enemy_state == near_enemy_state) {
        continue;
      }
      auto pos_b {near_enemy_state->model->position()};
      if (enemy_state->is_sibling_close(pos_b)) {
        sum += pos_a - pos_b;
      }
    }
    // found by experiments, it reduces force of attraction to the player and
    // it helps avoiding collapsing enemies during movement
    auto direction_weight = 0.05f;
    sum = glm::normalize(sum + glm::normalize(player->position() - pos_a) * direction_weight);
    enemy->move_in(sum, enemy_state->hunting_speed * meta.seconds_since_last_frame);
  }
}


void handle_enemy_sinking(GameState::Meta& meta) {
  auto filtered_enemies_state = filter_ptr<EnemyState>(
    meta.state.enemies_state(),
    [](EnemyState& v) { return v.state == EnemyState::SINKING && v.model->is_active(); }
  );
  for (auto& enemy_state : filtered_enemies_state) {
    auto enemy = enemy_state->model;
    enemy->move_in({0, 0, 1}, -1.5 * meta.seconds_since_last_frame);
    enemy->rotation_z(enemy->rotation_z() + 0.05);
    if (enemy->position().z < -2.0) {
      enemy->is_active(false);
      enemy_state->state = EnemyState::INACTIVE;
    }
  }
}


void handle_enemy_rotation(GameState::Meta& meta) {
  auto player_positon = meta.state.player()->position();
  for (auto& enemy_state : meta.state.enemies_state()) {
    auto enemy = enemy_state.model;
    if (enemy->is_active() && enemy_state.state == EnemyState::HUNTING) {
      auto direction_vector = player_positon - enemy->position();
      auto rotation = glm::atan(direction_vector.y, direction_vector.x);
      enemy->rotation_z(rotation);
    }
  }
}


void handle_particles(GameState::Meta& meta) {
  for (auto& particle : meta.state.particle_emitter().particles()) {
    auto& model = *particle.model;
    if (particle.current_live_time <= 0.0) {
      model.is_active(false);
      continue;
    }
    if (model.is_active()) {
      auto angle = model.rotation_z();
      auto direction = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0);
      model.move_in(direction, 10.0 * meta.seconds_since_last_frame);
    }
    particle.current_live_time -= meta.seconds_since_last_frame;
  }
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
    auto has_intersection = glm::intersectRayPlane(
      meta.camera.position(),
      ray,
      {0.0, 0.0, meta.state.player()->position().z},
      {0.0, 0.0, 1.0},
      intersection_distance
    );
    if (has_intersection) {
      meta.state.cursor(meta.camera.position() + ray * intersection_distance);
    }
  });
}


void move_camera(GameState::Meta& meta) {
  auto player_pos = meta.state.player()->position();
  auto camera_pos = meta.state.camera()->position();
  meta.state.camera()->position({player_pos.x, player_pos.y, camera_pos.z});
}
