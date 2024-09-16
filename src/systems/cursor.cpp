#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_projection.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/vec4.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/services.h"
#include "src/utils/mouse.h"

#include "cursor.h"

void
cursor_handler_system(Scene& scene)
{
  auto mouse_pos = mouse_position();
  auto& camera = scene.state().camera();
  auto screen_size = Services::app().screen_size();
  glm::vec4 viewport { 0, 0, screen_size.width, screen_size.height };
  auto projection_point = glm::unProject(
    { mouse_pos.x, screen_size.height - mouse_pos.y, 1.0 },
    camera.view(),
    camera.projection(),
    viewport);
  auto ray = glm::normalize(projection_point - camera.position());
  scene.state().registry().view<Transform, PlayerKind>().each(
    [&scene, &camera, &ray](auto& player_transform) {
      float intersection_distance = 0;
      auto has_intersection = glm::intersectRayPlane(
        camera.position(),
        ray,
        { 0.0, 0.0, player_transform.translation().z },
        { 0.0, 0.0, 1.0 },
        intersection_distance);
      if (has_intersection) {
        scene.state().cursor(camera.position() + ray * intersection_distance);
      }
    });
}
