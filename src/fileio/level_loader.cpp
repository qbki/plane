#include <format>
#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <vector>

#include "src/fileio/params/meta.h"
#include "src/fileio/params/strategies.h"
#include "src/game_state/factory.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/services/logger.h"
#include "src/utils/file_loaders.h"
#include "src/utils/result.h"
#include "src/utils/types.h"

#include "level_loader.h"
#include "params.h"
#include "json/mappers.h" // IWYU pragma: keep

import pln.utils.crash;

void
setup_camera(CameraParams& camera_params, Scene& scene)
{
  scene.state().camera().position(camera_params.position);
}

void
setup_boundaries(BoundaryParams& boundaries, Scene& scene)
{
  auto is_valid = boundaries.min.x < boundaries.max.x
                  && boundaries.min.y < boundaries.max.y
                  && boundaries.min.z < boundaries.max.z;
  if (!is_valid) {
    pln::utils::crash("Incorrect world boundaries: wrong ordering");
  }
  scene.state().world_bbox({
    .min = boundaries.min,
    .max = boundaries.max,
  });
}

ModelFactory::MakerFn
get_entity_maker(const PositionStrategy& strategy,
                 const EntityParamsMap& entities)
{
  ModelFactory::MakerFn maker = [&entities, strategy](auto&... args) {
    auto behaviour = get_behaviour(strategy);
    switch (behaviour) {
      case BehaviourEnum::ENEMY: {
        return ModelFactory::make_enemy(args...);
        break;
      }
      case BehaviourEnum::PLAYER: {
        return ModelFactory::make_player(args...);
        break;
      }
      case BehaviourEnum::STATIC: {
        auto entity_id = std::visit(
          Overloaded {
            [](auto&) -> std::string { pln::utils::crash("Strategy not supported"); },
            [](const PositionStrategyMany& value) { return value.entity_id; },
            [](const PositionStrategySingle& value) { return value.entity_id; },
            // Temporally, should be removed after completion of the level
            // editor
            [](const PositionStrategySquare& value) {
              return value.entity_ids[0];
            },
          },
          strategy);
        auto entity_params = entities.params(entity_id);
        if (std::holds_alternative<EntityParamsText>(entity_params)) {
          return ModelFactory::make_text(args...);
        }
        return ModelFactory::make_static(args...);
        break;
      }
      case BehaviourEnum::TUTORIAL_BUTTON: {
        return ModelFactory::make_tutorial_button(args...);
        break;
      }
      case BehaviourEnum::LIGHT: {
        auto entity_id = std::visit(
          Overloaded {
            [](auto&) -> std::string {
            pln::utils::crash("No a single strategy for a light behaviour, "
                              "it is not supported at this moment");
            },
            [](const PositionStrategySingle& value) { return value.entity_id; },
            [](const PositionStrategyVoid& value) { return value.entity_id; } },
          strategy);
        auto entity_params = entities.params(entity_id);
        if (std::holds_alternative<EntityParamsDirectionalLight>(
              entity_params)) {
          return ModelFactory::make_directional_light(args...);
        } else if (std::holds_alternative<EntityParamsPointLight>(
                     entity_params)) {
          return ModelFactory::make_point_light(args...);
        } else {
          pln::utils::crash(
            std::format("Unknown light type for the entity: {}", entity_id));
        }
        break;
      }
      default: {
        pln::utils::crash("Unknown behaviour type???");
        break;
      }
    }
  };
  return maker;
}

void
preload_models(const std::vector<EntityParamsModel>& models)
{
  for (auto& model : models) {
    Services::cache().get_model(model.path);
  }
}

void
load_level(const std::string& entities_file_path,
           const std::string& level_file_path,
           Scene& scene)
{
  auto entities = load_json(entities_file_path)
                    .or_crash()
                    .at("entities")
                    .get<EntityParamsMap>();
  auto json_level = load_json(level_file_path);
  auto meta = json_level.or_crash().at("meta").get<LevelMetaParams>();
  auto strategies = json_level.or_crash()
                      .at("map")
                      .get<std::vector<PositionStrategy>>();
  auto models = entities.get_all<EntityParamsModel>();
  preload_models(models);
  setup_camera(meta.camera, scene);
  setup_boundaries(meta.boundaries, scene);
  for (auto& strategy : strategies) {
    auto maker = get_entity_maker(strategy, entities);
    PositionStrategyVisitor::visit(strategy, entities, scene, maker);
  }
  Services::logger().info(std::format("Loaded level: {}", level_file_path));
}
