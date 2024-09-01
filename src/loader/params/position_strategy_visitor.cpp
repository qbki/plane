#include <cstddef>
#include <functional>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <ranges>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "src/components/linear_velocity.h"
#include "src/components/transform.h"
#include "src/game_state/factory.h"
#include "src/utils/random.h"

#include "components_attacher.h"
#include "entities_map.h"
#include "entity_maker.h"
#include "position_strategy_visitor.h"
#include "strategies.h"

PositionStrategyVisitor::PositionStrategyVisitor(
  const EntityParamsMap* entities,
  Scene* scene,
  ModelFactory::MakerFn* maker_fn)
  : _entities(entities)
  , _scene(scene)
  , _entity_maker(&scene->state().registry(), entities, maker_fn){};

entt::entity
PositionStrategyVisitor::handle_single(const std::string& entity_id) const
{
  auto entity_params = _entities->params(entity_id);
  auto entity = _entity_maker.get(entity_params);
  ComponetsAttacher(_scene, entity, _entities).attach(entity_params);
  return entity;
}

void
PositionStrategyVisitor::operator()(const PositionStrategyRound& strategy) const
{
  auto& registry = _scene->state().registry();
  auto& center = strategy.center;
  auto& radius = strategy.radius;
  auto speed_items_view =
    strategy.entity_ids | std::views::transform([&](const auto& id) {
      return _entities->actor(id).speed;
    });
  std::vector<float> speed_items(speed_items_view.begin(),
                                 speed_items_view.end());
  auto get_random_int =
    make_random_fn(static_cast<size_t>(0), strategy.entity_ids.size() - 1);
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
    auto entity_params = _entities->params(entity_id);
    auto entity = _entity_maker.get(entity_params);
    auto speed = speed_items.at(entity_index);
    ComponetsAttacher(_scene, entity, _entities).attach(entity_params);
    Transform transform;
    transform.translate(glm::vec3(center.x + static_cast<float>(coord.x),
                                  center.y + static_cast<float>(coord.y),
                                  center.z));
    registry.replace<Transform>(entity, transform);
    registry.replace<LinearVelocity>(entity, speed);
  }
}

void
PositionStrategyVisitor::operator()(
  const PositionStrategySingle& strategy) const
{
  auto& registry = _scene->state().registry();
  auto entity = handle_single(strategy.entity_id);
  Transform transform;
  transform.translate(strategy.position);
  registry.emplace_or_replace<Transform>(entity, transform);
}

void
PositionStrategyVisitor::operator()(const PositionStrategyMany& strategy) const
{
  auto& registry = _scene->state().registry();
  for (const auto& position : strategy.positions) {
    auto entity = handle_single(strategy.entity_id);
    Transform transform;
    transform.translate(position);
    registry.emplace_or_replace<Transform>(entity, transform);
  }
}

void
PositionStrategyVisitor::operator()(
  const PositionStrategySquare& strategy) const
{
  auto& registry = _scene->state().registry();
  auto& center = strategy.center;
  auto& width = strategy.width;
  auto& height = strategy.height;
  auto get_random_int =
    make_random_fn(static_cast<size_t>(0), strategy.entity_ids.size() - 1);
  const auto half = 2.0f;
  auto start_x = center.x - static_cast<float>(width) / half;
  auto start_y = center.y - static_cast<float>(height) / half;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      auto entity_index = get_random_int();
      auto entity_id = strategy.entity_ids.at(entity_index);
      auto entity_params = _entities->params(entity_id);
      auto entity = _entity_maker.get(entity_params);
      ComponetsAttacher(_scene, entity, _entities).attach(entity_params);
      Transform transform;
      transform.translate(glm::vec3(start_x + static_cast<float>(x),
                                    start_y + static_cast<float>(y),
                                    center.z));
      registry.replace<Transform>(entity, transform);
    }
  }
}

void
PositionStrategyVisitor::operator()(const PositionStrategyUndefined&) const
{
  throw std::runtime_error("Can't handle an unknown strategy");
}

void
PositionStrategyVisitor::operator()(const PositionStrategyVoid& strategy) const
{
  std::ignore = handle_single(strategy.entity_id);
}
