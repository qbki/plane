module;
#include <utility>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/math/intersection.h"

export module pln.systems.entities_collector;

import pln.collections.octree;
import pln.scene.iscene;

namespace pln::systems::entities_collector {

constexpr unsigned int MAX_OCTREE_DEPTH = 12;

export
void
collect_entities(pln::scene::IScene& scene)
{
  auto& registry = scene.state().registry();
  const auto& state = scene.state();
  auto octree = std::make_unique<pln::collections::Octree<entt::entity>>(state.world_bbox(),
                                                                         MAX_OCTREE_DEPTH);

  registry.view<Transform, MeshPointer, EnemyKind>().each(
    [&octree](entt::entity entity,
              const Transform& transform,
              const MeshPointer& mesh) {
      auto collider = apply_transform_to_collider(transform,
                                                  mesh->bounding_volume());
      octree->insert(std::get<Shape::Sphere>(collider), entity);
    });

  scene.entities(std::move(octree));
}

}
