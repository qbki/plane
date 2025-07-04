module;
#include <entt/entt.hpp>
#include <glm/geometric.hpp>
#include <variant>

export module pln.systems.collision;

import pln.components.common;
import pln.components.transform;
import pln.consts;
import pln.math.intersections;
import pln.math.shapes;
import pln.scene.iscene;
import pln.services.app;

using namespace pln::components;
using namespace pln::math;

namespace pln::systems::collision {

export
void
collision(pln::scene::IScene& scene)
{
  auto& registry = scene.state().registry();
  auto delta_time = pln::services::app().delta_time();

  registry.view<Transform, Velocity, ActorKind>().each(
    [&](entt::entity entity, Transform& transform_a, Velocity& velocity_a) {
      auto collider_a = apply_transform_to_collider(transform_a,
                                                    pln::consts::DEFAULT_COLLIDER);
      auto shape_a = std::get_if<pln::math::Sphere>(&collider_a);
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
              velocity_b,
              mesh_b] = registry
                          .get<Transform, Velocity, MeshPointer>(near_entity);
        auto collider_b = apply_transform_to_collider(transform_b,
                                                      pln::consts::DEFAULT_COLLIDER);
        auto shape_b = std::get_if<pln::math::Sphere>(&collider_b);
        if (!shape_b) {
          continue;
        }
        auto ab_direction = shape_a->center - shape_b->center;
        auto norm_direction = glm::normalize(ab_direction);
        auto distance = glm::length(ab_direction);
        auto radius_sum = shape_a->radius + shape_b->radius;
        if (distance > radius_sum) {
          continue;
        }

        velocity_a.value = glm::reflect(velocity_a.value, norm_direction)
                           * pln::consts::HALF;
        const float offset = radius_sum + 0.01f;
        transform_a.translate(shape_b->center + norm_direction * offset);

        velocity_b.value += -velocity_a.value;
      }
    });
}

}
