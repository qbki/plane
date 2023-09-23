#include <functional>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

#include "./index.h"


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


void move_player(Scene::Meta& meta) {
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
  meta.scene.player()->move_in(direction, 5.0 * meta.seconds_since_last_frame);
}


void rotate_player(Scene::Meta& meta) {
  auto player = meta.scene.player();
  auto direction = meta.scene.cursor() - player->position();
  auto angle = glm::atan(direction.y, direction.x);
  player->rotation_z(angle);
}


void shoot_by_player(Scene::Meta& meta) {
  const auto& player = meta.scene.player();
  if (meta.control.is_player_shooting) {
    for (auto& bullet : meta.scene.bullets()) {
      if (!bullet->is_active()) {
        bullet->is_active(true);
        bullet->position(player->position());
        bullet->rotation_z(player->rotation_z());
        break;
      }
    }
  }
};


void handle_bullets (Scene::Meta& meta) {
  for (auto& bullet : meta.scene.bullets()) {
    if (!bullet->is_active()) {
      continue;
    }

    auto angle = bullet->rotation_z();
    auto directin = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0);
    bullet->move_in(directin, 30.0 * meta.seconds_since_last_frame);
    auto pos = bullet->position();
    if (pos.x < -12 || pos.x > 12 || pos.y < -6 || pos.y > 6) {
      bullet->is_active(false);
    }

    for (auto& enemy_state : meta.scene.enemies_state()) {
      auto enemy = enemy_state.model;
      if (glm::distance(bullet->position(), enemy->position()) <= 0.3) {
        if (enemy_state.state != EnemyState::SINKING) {
          enemy_state.state = EnemyState::SINKING;
        }
        bullet->is_active(false);
      }
    }
  }
}

void handle_enemies_hunting(Scene::Meta& meta) {
  auto player = meta.scene.player();
  auto filtered_enemies_state = filter_ptr<EnemyState>(
    meta.scene.enemies_state(),
    [](EnemyState& v) { return v.state == EnemyState::HUNTING; }
  );
  for (auto& enemy_state : filtered_enemies_state) {
    auto enemy = enemy_state->model;
    auto pos_a {enemy->position()};
    glm::vec3 sum {0, 0, 0};
    for (auto& near_enemy_state : filtered_enemies_state) {
      if (&enemy_state == &near_enemy_state) {
        continue;
      }
      auto pos_b {near_enemy_state->model->position()};
      if (glm::distance(pos_a, pos_b) < 0.7) {
        sum = glm::normalize(sum + glm::normalize(pos_a - pos_b));
      }
    }
    sum = glm::normalize(sum + glm::normalize(player->position() - pos_a) * 0.05f);
    enemy->move_in(sum, 1.0 * meta.seconds_since_last_frame);
  }
}

void handle_enemy_sinking(Scene::Meta& meta) {
  auto filtered_enemies_state = filter_ptr<EnemyState>(
    meta.scene.enemies_state(),
    [](EnemyState& v) { return v.state == EnemyState::SINKING && v.model->is_active(); }
  );
  for (auto& enemy_state : filtered_enemies_state) {
    auto enemy = enemy_state->model;
    enemy->move_in({0, 0, 1}, -1.5 * meta.seconds_since_last_frame);
    enemy->rotation_z(enemy->rotation_z() + 0.05);
    if (enemy->position().z < -2.0) {
      enemy->is_active(false);
    }
  }
}

void handle_enemy_rotation(Scene::Meta& meta) {
  auto player_positon = meta.scene.player()->position();
  for (auto& enemy_state : meta.scene.enemies_state()) {
    auto enemy = enemy_state.model;
    if (enemy->is_active() && enemy_state.state == EnemyState::HUNTING) {
      auto direction_vector = player_positon - enemy->position();
      auto rotation = glm::atan(direction_vector.y, direction_vector.x);
      enemy->rotation_z(rotation);
    }
  }
}

Scene::Handler make_cursor_handler(int& screen_width, int& screen_height) {
  return ([&screen_width, &screen_height](Scene::Meta meta) {
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
      {0.0, 0.0, meta.scene.player()->position().z},
      {0.0, 0.0, 1.0},
      intersection_distance
    );
    if (has_intersection) {
      meta.scene.cursor(meta.camera.position() + ray * intersection_distance);
    }
  });
}
