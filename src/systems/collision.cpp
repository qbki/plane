#include <glm/geometric.hpp>
#include <variant>

#include "src/collections/octree.h"
#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/consts.h"
#include "src/math/intersection.h"
#include "src/services.h"

#include "collision.h"

void
collision_system(Scene& scene)
{
  auto& registry = scene.state().registry();
  auto delta_time = Services::app().delta_time();

  registry.view<Transform, Velocity, PlayerKind>().each(
    [&](entt::entity entity, Transform& transform_a, Velocity& velocity_a) {
      auto collider_a = apply_transform_to_collider(transform_a,
                                                    DEFAULT_COLLIDER);
      auto shape_a = std::get_if<Shape::Sphere>(&collider_a);
      if (!shape_a) {
        return;
      }
      auto query = *shape_a;
      query.radius += glm::length(velocity_a.value) * delta_time;
      auto near_entities = scene.entities()->at(query);
      for (auto near_entity : near_entities) {
        if (near_entity == entity) {
          continue;
        }
        auto [transform_b,
              mesh_b] = registry.get<Transform, MeshPointer>(near_entity);
        auto collider_b = apply_transform_to_collider(transform_b,
                                                      DEFAULT_COLLIDER);
        auto shape_b = std::get_if<Shape::Sphere>(&collider_b);
        if (!shape_b) {
          continue;
        }
        auto ab_direction = shape_a->center - shape_b->center;
        auto norm_direction = glm::normalize(ab_direction);
        auto distance = glm::length(ab_direction);
        auto radius_sum = shape_a->radius + shape_b->radius;
        if (distance >= radius_sum) {
          continue;
        }
        velocity_a.value = glm::reflect(velocity_a.value, norm_direction);
        transform_a.translate(shape_b->center + norm_direction * radius_sum);
      }
    });
}
