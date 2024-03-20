#include <cstddef>
#include <functional>
#include <glm/ext/vector_int2.hpp>
#include <glm/geometric.hpp>
#include <nlohmann/json.hpp>
#include <ranges>
#include <string>
#include <vector>

#include "src/app.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/game_state/factory.h"
#include "src/loader/params.h"
#include "src/utils/random.h"

#include "attachers.h"
#include "mappers.h" // IWYU pragma: keep
#include "setups.h"

void
single_strategy(const nlohmann::basic_json<>& json_entities,
                const nlohmann::basic_json<>& json_strategy,
                App& app,
                ModelFactory::MakerFn maker_fn)
{
  auto& registry = app.game_state->registry();
  auto json_entity_id = json_strategy.at("entity_id").get<std::string>();
  auto json_entity = json_entities.at(json_entity_id);
  auto model_path =
    json_entity.value<std::string>("model", "[path not defined]");
  auto entity = maker_fn(registry, model_path);
  attach_color(json_entity, registry, entity);
  attach_direction(json_entity, registry, entity);
  attach_opaque(json_entity, registry, entity);
  attach_transform(json_strategy, registry, entity);
  attach_projectile_emmiter(json_entities, json_entity, app, entity);
  attach_velocity(json_entity, registry, entity);
  attach_tutorial_button_value(json_strategy, registry, entity);
  setup_player(json_strategy, app, entity);
}

void
square_strategy(const nlohmann::basic_json<>& json_entities,
                const nlohmann::basic_json<>& json_strategy,
                App& app,
                ModelFactory::MakerFn maker_fn)
{
  auto& registry = app.game_state->registry();
  auto strategy_params = json_strategy.get<SquareStrategyParams>();
  auto& center = strategy_params.center;
  auto& entity_ids = strategy_params.entity_ids;
  auto& width = strategy_params.width;
  auto& height = strategy_params.height;
  auto get_random_int =
    make_random_fn(static_cast<size_t>(0), entity_ids.size() - 1);
  const auto half = 2.0f;
  auto start_x = center.x - static_cast<float>(width) / half;
  auto start_y = center.y - static_cast<float>(height) / half;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      auto entity_index = get_random_int();
      auto entity_id = entity_ids.at(entity_index);
      auto json_entity = json_entities.at(entity_id);
      auto model_path = json_entity.at("model").get<std::string>();
      auto entity = maker_fn(registry, model_path);
      Transform transform;
      transform.translate(glm::vec3(start_x + static_cast<float>(x),
                                    start_y + static_cast<float>(y),
                                    center.z));
      registry.replace<Transform>(entity, transform);
    }
  }
}

void
round_strategy(const nlohmann::basic_json<>& json_entities,
               const nlohmann::basic_json<>& json_strategy,
               App& app,
               ModelFactory::MakerFn maker_fn)
{
  auto& registry = app.game_state->registry();
  auto strategy_params = json_strategy.get<RoundStrategyParams>();
  auto& center = strategy_params.center;
  auto& entity_ids = strategy_params.entity_ids;
  auto& radius = strategy_params.radius;
  auto velocity_items_view =
    entity_ids | std::views::transform([&](auto& id) {
      return json_entities.at(id).at("velocity").template get<VelocityParams>();
    });
  std::vector<VelocityParams> velocity_items(velocity_items_view.begin(),
                                             velocity_items_view.end());
  auto get_random_int =
    make_random_fn(static_cast<size_t>(0), entity_ids.size() - 1);
  auto radius_float = static_cast<float>(radius);

  std::vector<glm::ivec2> coords;
  for (int x = -radius; x < radius; x++) {
    for (int y = -radius; y < radius; y++) {
      auto distance = glm::length(glm::vec2(x, y));
      if (distance < radius_float) {
        coords.emplace_back(x, y);
      }
    }
  }

  for (auto& coord : coords) {
    auto entity_index = get_random_int();
    auto entity_id = entity_ids.at(entity_index);
    auto json_entity = json_entities.at(entity_id);
    auto model_path = json_entity.at("model").get<std::string>();
    auto velocity = velocity_items.at(entity_index);
    auto entity = maker_fn(registry, model_path);
    Transform transform;
    transform.translate(glm::vec3(center.x + static_cast<float>(coord.x),
                                  center.y + static_cast<float>(coord.y),
                                  center.z));
    registry.replace<Transform>(entity, transform);
    registry.replace<Velocity>(entity, velocity.acceleration, velocity.damping);
    attach_particles_emmiter_by_hit(
      json_entities.at(json_strategy.at("hit_particle")), app, entity);
  }
}
