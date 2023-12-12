#include <glm/gtx/intersect.hpp>

#include "./cursor.h"


void cursor_handler_system(App::Meta& meta) {
  auto mouse_pos = meta.app.mouse_position();
  auto camera = meta.app.game_state->camera();
  auto& screen_size = meta.app.screen_size;
  glm::vec4 viewport {0, 0, screen_size->width, screen_size->height};
  auto projection_point = glm::unProject(
    {mouse_pos.x, screen_size->height - mouse_pos.y, 1.0},
    camera->view(),
    camera->projection(),
    viewport
  );
  auto ray = glm::normalize(projection_point - camera->position());
  float intersection_distance = 0;
  auto [player_position] = meta.app.game_state->player<Position>();

  auto has_intersection = glm::intersectRayPlane(
    camera->position(),
    ray,
    {0.0, 0.0, player_position.z},
    {0.0, 0.0, 1.0},
    intersection_distance
  );
  if (has_intersection) {
    meta.app.game_state->cursor(camera->position() + ray * intersection_distance);
  }
}
