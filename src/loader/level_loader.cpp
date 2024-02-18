#include <entt/entt.hpp>
#include <functional>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <string>

#include "src/game_state/factory.h"
#include "src/utils/file_loaders.h"
#include "src/utils/mappers.h"

#include "level_loader.h"
#include "json/extractors.h"
#include "json/setups.h"
#include "json/strategies.h"

ModelFactory::MakerFn
map_maker(std::string key,
          std::unordered_map<std::string, ModelFactory::MakerFn>&& values)
{
  return map_value(key, std::move(values));
}

std::function<decltype(single_strategy)>
map_strategy(
  std::string key,
  std::unordered_map<std::string, std::function<decltype(single_strategy)>>&&
    values)
{
  return map_value(key, std::move(values));
}

ModelFactory::MakerFn
get_entity_maker(App& app,
                 const nlohmann::basic_json<>& strategy_node,
                 const nlohmann::basic_json<>& entities_node)
{
  auto strategy_behaviour = strategy_node.at("behaviour").get<std::string>();
  auto& factory = app.game_state->factory();
  return map_maker(
    strategy_behaviour,
    { { "player",
        [&](auto&&... args) { return factory.make_player(args...); } },
      { "static",
        [&](auto&&... args) { return factory.make_static(args...); } },
      { "enemy", [&](auto&&... args) { return factory.make_enemy(args...); } },
      { "light", [&](auto&&... args) {
         auto entity_kind = extract_kind(strategy_node, entities_node);
         auto make =
           map_maker(entity_kind,
                     { { "directional_light",
                         [&](auto&&... args) {
                           return factory.make_directional_light(args...);
                         } },
                       { "point_light", [&](auto&&... args) {
                          return factory.make_point_light(args...);
                        } } });
         return make(args...);
       } } });
}

void
load_level(const std::string& entities_file_path,
           const std::string& level_file_path,
           App& app)
{
  auto json_entities = load_json(entities_file_path).at("entities");
  auto json_level = load_json(level_file_path);

  setup_camera(json_level, app);
  for (auto& json_strategy : json_level.at("map")) {
    auto strategy_name =
      json_strategy.at("position_strategy").get<std::string>();
    auto behaviour_name = json_strategy.at("behaviour").get<std::string>();
    auto maker = get_entity_maker(app, json_strategy, json_entities);
    auto run_strategy = map_strategy(strategy_name,
                                     {
                                       { "single", single_strategy },
                                       { "round", round_strategy },
                                       { "square", square_strategy },
                                       { "void", single_strategy },
                                     });
    run_strategy(json_entities, json_strategy, app, maker);
  }
}
