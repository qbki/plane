#pragma once
#include <string>

#include "src/game_state/factory.h"
#include "src/scene/scene.h"

#include "entities_map.h"
#include "entity_maker.h"
#include "strategies.h"

class PositionStrategyVisitor
{
private:
  const EntityParamsMap* _entities;
  Scene* _scene;
  EntityMaker _entity_maker;

  [[nodiscard]] entt::entity handle_single(const std::string& entity_id) const;

public:
  PositionStrategyVisitor(const EntityParamsMap* entities,
                          Scene* scene,
                          ModelFactory::MakerFn* maker_fn);
  void operator()(const PositionStrategyRound& strategy) const;
  void operator()(const PositionStrategySingle& strategy) const;
  void operator()(const PositionStrategyMany& strategy) const;
  void operator()(const PositionStrategySquare& strategy) const;
  void operator()(const PositionStrategyUndefined& strategy) const;
  void operator()(const PositionStrategyVoid& strategy) const;
};
