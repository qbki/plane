#include <glm/ext/matrix_projection.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/vec4.hpp>

#include "src/components/transform.h"

#include "cursor.h"

void
cursor_handler_system(App::Meta& meta)
{
  auto mouse_pos = meta.app->mouse_position();
  auto& camera = meta.app->game_state->camera();
  auto& screen_size = meta.app->screen_size;
  glm::vec4 viewport{ 0, 0, screen_size->width, screen_size->height };
  auto projection_point = glm::unProject(
    { mouse_pos.x, static_cast<float>(screen_size->height) - mouse_pos.y, 1.0 },
    camera->view(),
    camera->projection(),
    viewport);
  auto ray = glm::normalize(projection_point - camera->position());
  float intersection_distance = 0;
  auto player_transform = meta.app->game_state->player<Transform>();

  auto has_intersection =
    glm::intersectRayPlane(camera->position(),
                           ray,
                           { 0.0, 0.0, player_transform.translation().z },
                           { 0.0, 0.0, 1.0 },
                           intersection_distance);
  if (has_intersection) {
    meta.app->game_state->cursor(camera->position() +
                                 ray * intersection_distance);
  }
}
