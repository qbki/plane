#include <glm/gtx/intersect.hpp>

#include "./cursor.h"


GameState::Handler make_cursor_handler_system(int& screen_width, int& screen_height) {
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
