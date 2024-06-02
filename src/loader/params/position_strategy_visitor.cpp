#include <cstddef>
#include <functional>
#include <glm/geometric.hpp>
#include <ranges>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "src/app/app.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/game_state/factory.h"
#include "src/utils/random.h"

#include "components_attacher.h"
#include "entities_map.h"
#include "entity_maker.h"
#include "position_strategy_visitor.h"
#include "strategies.h"

void
setup_player(BehaviourEnum behaviour, const App& app, entt::entity entity)
{
  if (behaviour == BehaviourEnum::PLAYER) {
    app.game_state().player_id(entity);
  }
}

PositionStrategyVisitor::PositionStrategyVisitor(
  const EntityParamsMap* entities,
  const App* app,
  ModelFactory::MakerFn* maker_fn)
  : _entities(entities)
  , _app(app)
  , _entity_maker(&app->game_state().registry(), entities, maker_fn){};

entt::entity
PositionStrategyVisitor::handle_single(const std::string& entity_id) const
{
  auto entity_params = _entities->params(entity_id);
  auto entity = _entity_maker.get(entity_params);
  ComponetsAttacher(_app, entity, _entities).attach(entity_params);
  return entity;
}

void
PositionStrategyVisitor::operator()(const PositionStrategyRound& strategy) const
{
  auto& registry = _app->game_state().registry();
  auto& center = strategy.center;
  auto& radius = strategy.radius;
  auto velocity_items_view =
    strategy.entity_ids | std::views::transform([&](const auto& id) {
      return _entities->actor(id).velocity;
    });
  std::vector<VelocityParams> velocity_items(velocity_items_view.begin(),
                                             velocity_items_view.end());
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
    auto velocity = velocity_items.at(entity_index);
    ComponetsAttacher(_app, entity, _entities).attach(entity_params);
    Transform transform;
    transform.translate(glm::vec3(center.x + static_cast<float>(coord.x),
                                  center.y + static_cast<float>(coord.y),
                                  center.z));
    registry.replace<Transform>(entity, transform);
    registry.replace<Velocity>(entity, velocity.acceleration, velocity.damping);
  }
}

void
PositionStrategyVisitor::operator()(
  const PositionStrategySingle& strategy) const
{
  auto& registry = _app->game_state().registry();
  auto entity = handle_single(strategy.entity_id);
  Transform transform;
  transform.translate(strategy.position);
  registry.emplace_or_replace<Transform>(entity, transform);
  setup_player(strategy.behaviour, *_app, entity);
}

void
PositionStrategyVisitor::operator()(
  const PositionStrategySquare& strategy) const
{
  auto& registry = _app->game_state().registry();
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
      ComponetsAttacher(_app, entity, _entities).attach(entity_params);
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
