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

  [[nodiscard]] entt::entity handle_single(const std::string& entity_id);

public:
  PositionStrategyVisitor(const EntityParamsMap* entities,
                          Scene* scene,
                          ModelFactory::MakerFn* maker_fn);
  void operator()(const PositionStrategyRound& strategy);
  void operator()(const PositionStrategySingle& strategy);
  void operator()(const PositionStrategyMany& strategy);
  void operator()(const PositionStrategySquare& strategy);
  void operator()(const PositionStrategyUndefined& strategy) const;
  void operator()(const PositionStrategyVoid& strategy);
};
