#include <utility>
#include <variant>

#include "src/collections/octree.h"
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/math/intersection.h"

#include "entities_collector.h"

const unsigned int MAX_OCTREE_DEPTH = 12;

void
collect_entities_system(Scene& scene)
{
  auto& registry = scene.state().registry();
  const auto& state = scene.state();
  auto octree = std::make_unique<Octree<entt::entity>>(state.world_bbox(),
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
