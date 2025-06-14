#include <cstddef>
#include <format>
#include <functional>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <ranges>
#include <string>
#include <tuple>
#include <utility>
#include <variant>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/game_state/factory.h"
#include "src/utils/random.h"

#include "components_attacher.h"
#include "entities_map.h"
#include "entity_maker.h"
#include "position_strategy_visitor.h"
#include "strategies.h"

import pln.consts;
import utils.crash;

PositionStrategyVisitor::PositionStrategyVisitor(
  std::reference_wrapper<const EntityParamsMap> entities,
  std::reference_wrapper<Scene> scene,
  ModelFactory::MakerFn maker_fn)
  : _entities(entities)
  , _scene(scene)
  , _maker_fn(std::move(maker_fn)) {};

entt::entity
PositionStrategyVisitor::handle_single(const std::string& entity_id)
{
  auto entity_params = _entities.get().params(entity_id);
  auto registry = _scene.get().state().shared_registry();
  auto entity = EntityMaker::get(registry, _entities, _maker_fn, entity_params);
  ComponetsAttacher::attach(_scene, entity, _entities, entity_params);
  return entity;
}

void
PositionStrategyVisitor::operator()(const PositionStrategyRound& strategy)
{
  auto registry = _scene.get().state().shared_registry();
  auto& center = strategy.center;
  auto& radius = strategy.radius;
  auto speed_items_view = strategy.entity_ids
                          | std::views::transform([&](const auto& id) {
                              return _entities.get().actor(id).speed;
                            });
  std::vector<VelocityParams> speed_items(speed_items_view.begin(),
                                          speed_items_view.end());
  auto get_random_int = make_random_fn(static_cast<size_t>(0),
                                       strategy.entity_ids.size() - 1);
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
    auto entity_id = strategy.entity_ids.at(entity_index);
    auto entity_params = _entities.get().params(entity_id);
    auto entity = EntityMaker::get(
      registry, _entities, _maker_fn, entity_params);
    auto speed = speed_items.at(entity_index);
    ComponetsAttacher::attach(_scene, entity, _entities, entity_params);
    Transform transform;
    transform.translate(glm::vec3(center.x + static_cast<float>(coord.x),
                                  center.y + static_cast<float>(coord.y),
                                  center.z));
    registry->replace<Transform>(entity, transform);
    if (speed.speed.has_value()) {
      registry->replace<Speed>(entity, speed.speed.value());
    }
  }
}

void
PositionStrategyVisitor::operator()(const PositionStrategySingle& strategy)
{
  auto& registry = _scene.get().state().registry();
  auto entity = handle_single(strategy.entity_id);
  auto transform = registry.try_get<Transform>(entity);
  if (transform == nullptr) {
    constexpr auto message = "Entity {} can't be placed in exact position";
    pln::utils::crash(std::format(message, strategy.entity_id));
  }
  transform->translate(strategy.position);
}

void
PositionStrategyVisitor::operator()(const PositionStrategyMany& strategy)
{
  auto& registry = _scene.get().state().registry();
  for (const auto& position : strategy.positions) {
    auto entity = handle_single(strategy.entity_id);
    auto& transform = registry.get<Transform>(entity);
    transform.translate(position);
  }
}

void
PositionStrategyVisitor::operator()(const PositionStrategySquare& strategy)
{
  auto registry = _scene.get().state().shared_registry();
  auto& center = strategy.center;
  auto& width = strategy.width;
  auto& height = strategy.height;
  auto get_random_int = make_random_fn(static_cast<size_t>(0),
                                       strategy.entity_ids.size() - 1);
  auto start_x = center.x - static_cast<float>(width) * pln::consts::HALF;
  auto start_y = center.y - static_cast<float>(height) * pln::consts::HALF;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      auto entity_index = get_random_int();
      auto entity_id = strategy.entity_ids.at(entity_index);
      auto entity_params = _entities.get().params(entity_id);
      auto entity = EntityMaker::get(
        registry, _entities, _maker_fn, entity_params);
      ComponetsAttacher::attach(_scene, entity, _entities, entity_params);
      Transform transform;
      transform.translate(glm::vec3(start_x + static_cast<float>(x),
                                    start_y + static_cast<float>(y),
                                    center.z));
      registry->replace<Transform>(entity, transform);
    }
  }
}

void
PositionStrategyVisitor::operator()(const PositionStrategyUndefined&) const
{
  pln::utils::crash("Can't handle an unknown strategy");
}

void
PositionStrategyVisitor::operator()(const PositionStrategyVoid& strategy)
{
  std::ignore = handle_single(strategy.entity_id);
}

void
PositionStrategyVisitor::visit(const PositionStrategy& strategy,
                               const EntityParamsMap& entities,
                               Scene& scene,
                               const ModelFactory::MakerFn& maker_fn)
{
  PositionStrategyVisitor strategy_handler(entities, scene, maker_fn);
  std::visit(strategy_handler, strategy);
}
