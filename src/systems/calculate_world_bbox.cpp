#define GLM_ENABLE_EXPERIMENTAL
#include <format>
#include <glm/gtx/string_cast.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/math/intersection.h"
#include "src/math/shapes.h"
#include "src/services.h"

#include "calculate_world_bbox.h"

void
calculate_world_bbox(const App::Meta& meta)
{
  auto& game_state = meta.app->game_state;
  auto& registry = game_state->registry();
  registry.view<MeshPointer, Transform>().each(
    [&](const MeshPointer& mesh, const Transform& transform) {
      auto bounding_volume =
        apply_transform_to_collider(transform, mesh->bounding_volume());
      game_state->extend_world_bbox(bounding_volume);
    });
  auto world_bbox = game_state->world_bbox();
  logger().info(std::format("World bounding box: {} x {}",
                            glm::to_string(world_bbox.min),
                            glm::to_string(world_bbox.max)));
  logger().info(std::format("World size: {}",
                            glm::to_string(world_bbox.max - world_bbox.min)));
}
