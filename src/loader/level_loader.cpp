#include <format>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "src/app.h"
#include "src/game_state/factory.h"
#include "src/models/cache.h"
#include "src/utils/file_loaders.h"
#include "src/utils/types.h"

#include "level_loader.h"
#include "params.h"
#include "json/mappers.h" // IWYU pragma: keep

void
setup_camera(CameraParams camera_params, App& app)
{
  app.game_state->camera()->position(camera_params.position);
}

ModelFactory::MakerFn
get_entity_maker(App& app,
                 const PositionStrategy& strategy,
                 const EntityParamsMap& entities)
{
  auto& factory = app.game_state->factory();
  ModelFactory::MakerFn maker = [&factory, &entities, strategy](auto&... args) {
    auto behaviour = get_behaviour(strategy);
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
        auto entity_id = std::visit(
          Overloaded{
            [](auto&) -> std::string {
              throw std::runtime_error("Non single strategy for light behaviour"
                                       " is not supported at this moment");
            },
            [](const PositionStrategySingle& value) { return value.entity_id; },
            [](const PositionStrategyVoid& value) { return value.entity_id; } },
          strategy);
        auto entity_params = entities.params(entity_id);
        if (std::holds_alternative<EntityParamsDirectionalLight>(
              entity_params)) {
          return factory.make_directional_light(args...);
        } else if (std::holds_alternative<EntityParamsPointLight>(
                     entity_params)) {
          return factory.make_point_light(args...);
        } else {
          throw std::runtime_error(
            std::format("Unknown light type for the entity: {}", entity_id));
        }
        break;
      }
      default: {
        throw std::runtime_error("Unknown behaviour type???");
        break;
      }
    }
  };
  return maker;
}

void
preload_models(const std::vector<EntityParamsModel>& models, App& app)
{
  auto& factory = app.game_state->factory();
  for (auto& model : models) {
    factory.cache().load(model.path);
  }
}

void
load_level(const std::string& entities_file_path,
           const std::string& level_file_path,
           App& app)
{
  auto entities =
    load_json(entities_file_path).at("entities").get<EntityParamsMap>();
  auto json_level = load_json(level_file_path);
  auto camera = json_level.at("camera").get<CameraParams>();
  auto strategies = json_level.at("map").get<std::vector<PositionStrategy>>();
  auto models = entities.get_all<EntityParamsModel>();
  preload_models(models, app);
  setup_camera(camera, app);
  for (auto& strategy : strategies) {
    auto maker = get_entity_maker(app, strategy, entities);
    PositionStrategyVisitor strategy_handler(&entities, &app, &maker);
    std::visit(strategy_handler, strategy);
  }
}
