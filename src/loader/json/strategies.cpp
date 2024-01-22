#include "src/components/common.h"
#include "src/components/velocity.h"
#include "src/utils/random.h"

#include "attachers.h"
#include "extractors.h"
#include "setups.h"
#include "strategies.h"

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
  attach_position(json_strategy, registry, entity);
  attach_projectile_emmiter(json_entities, json_entity, app, entity);
  attach_velocity(json_entity, registry, entity);
  setup_player(json_strategy, app, entity);
}

void
square_strategy(const nlohmann::basic_json<>& json_entities,
                const nlohmann::basic_json<>& json_strategy,
                App& app,
                ModelFactory::MakerFn maker_fn)
{
  auto& registry = app.game_state->registry();
  auto entity_ids = extract_vector<std::string>(json_strategy.at("entity_ids"));
  auto get_random_int =
    make_random_fn(static_cast<size_t>(0), entity_ids.size() - 1);
  auto width = json_strategy.at("width").get<int>();
  auto height = json_strategy.at("height").get<int>();
  auto center = extract_vec3(json_strategy.at("center"));
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
      registry.replace<Position>(entity,
                                 glm::vec3(start_x + static_cast<float>(x),
                                           start_y + static_cast<float>(y),
                                           center.z));
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
  auto entity_ids = extract_vector<std::string>(json_strategy.at("entity_ids"));
  std::vector<ExtractVelocityResult> velocity_items;
  std::ranges::copy(entity_ids | std::ranges::views::transform([&](auto& id) {
                      return extract_velocity(json_entities.at(id));
                    }),
                    std::back_inserter(velocity_items));
  auto get_random_int =
    make_random_fn(static_cast<size_t>(0), entity_ids.size() - 1);
  auto radius = json_strategy.at("radius").get<int>();
  auto center = extract_vec3(json_strategy.at("center"));
  auto radius_float = static_cast<float>(radius);
  for (int x = -radius; x < radius; x++) {
    for (int y = -radius; y < radius; y++) {
      auto distance = glm::length(glm::vec2(x, y));
      if (distance < radius_float) {
        auto entity_index = get_random_int();
        auto entity_id = entity_ids.at(entity_index);
        auto json_entity = json_entities.at(entity_id);
        auto model_path = json_entity.at("model").get<std::string>();
        auto velocity = velocity_items.at(entity_index);
        auto entity = maker_fn(registry, model_path);
        registry.replace<Position>(entity,
                                   glm::vec3(center.x + static_cast<float>(x),
                                             center.y + static_cast<float>(y),
                                             center.z));
        registry.replace<Velocity>(
          entity, velocity.acceleration, velocity.damping);
        attach_particles_emmiter_by_hit(
          json_entities.at(json_strategy.at("hit_particle")), app, entity);
      }
    }
  }
}
