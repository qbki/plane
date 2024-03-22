#include <format>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <variant>

#include "src/app.h"
#include "src/game_state/factory.h"
#include "src/loader/params.h"
#include "src/models/cache.h"
#include "src/utils/file_loaders.h"

#include "level_loader.h"
#include "json/extractors.h"
#include "json/mappers.h" // IWYU pragma: keep
#include "json/setups.h"
#include "json/strategies.h"

ModelFactory::MakerFn
get_entity_maker(App& app,
                 const nlohmann::basic_json<>& strategy_node,
                 const nlohmann::basic_json<>& entities_node)
{
  auto behaviour = strategy_node.at("behaviour").get<BehaviourEnum>();
  auto& factory = app.game_state->factory();
  ModelFactory::MakerFn maker = [&](auto&... args) {
    switch (behaviour) {
      case BehaviourEnum::ENEMY: {
        return factory.make_enemy(args...);
        break;
      }
      case BehaviourEnum::PLAYER: {
        return factory.make_player(args...);
        break;
      }
      case BehaviourEnum::STATIC: {
        return factory.make_static(args...);
        break;
      }
      case BehaviourEnum::TUTORIAL_BUTTON: {
        return factory.make_tutorial_button(args...);
        break;
      }
      case BehaviourEnum::LIGHT: {
        auto entity_kind = extract_kind(strategy_node, entities_node);
        if (entity_kind == "directional_light") {
          return factory.make_directional_light(args...);
        } else if (entity_kind == "point_light") {
          return factory.make_point_light(args...);
        } else {
          throw std::runtime_error(
            std::format("Unknown light type: {}", entity_kind));
        }
        break;
      }
      default: {
        auto behaviour_str = strategy_node.at("behaviour").get<std::string>();
        throw std::runtime_error(
          std::format("Unknown befaviour type: {}", behaviour_str));
        break;
      }
    }
  };
  return maker;
}

void
preload_models(const nlohmann::basic_json<>& entities_json, App& app)
{
  auto& factory = app.game_state->factory();
  for (auto& [_, value] : entities_json.items()) {
    if (value.contains("model")) {
      auto path = value.at("model").get<std::string>();
      factory.cache().load(path);
    }
  }
}

void
load_level(const std::string& entities_file_path,
           const std::string& level_file_path,
           App& app)
{
  auto json_entities = load_json(entities_file_path).at("entities");
  auto json_level = load_json(level_file_path);

  preload_models(json_entities, app);

  setup_camera(json_level, app);
  for (auto& json_strategy : json_level.at("map")) {
    auto maker = get_entity_maker(app, json_strategy, json_entities);
    auto strategy = json_strategy.get<PositionStrategy>();
    PositionStrategyVisitor strategy_handler(
      &json_entities, &json_strategy, &app, &maker);
    std::visit(strategy_handler, strategy);
  }
}
