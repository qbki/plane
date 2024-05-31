#pragma once
#include <string>

#include "src/app/app.h"
#include "src/game_state/factory.h"

#include "entities_map.h"
#include "entity_maker.h"
#include "strategies.h"

class PositionStrategyVisitor
{
private:
  const EntityParamsMap* _entities;
  const App* _app;
  EntityMaker _entity_maker;

  [[nodiscard]] entt::entity handle_single(const std::string& entity_id) const;

public:
  PositionStrategyVisitor(const EntityParamsMap* entities,
                          const App* app,
                          ModelFactory::MakerFn* maker_fn);
  void operator()(const PositionStrategyRound& strategy) const;
  void operator()(const PositionStrategySingle& strategy) const;
  void operator()(const PositionStrategySquare& strategy) const;
  void operator()(const PositionStrategyUndefined& strategy) const;
  void operator()(const PositionStrategyVoid& strategy) const;
};
