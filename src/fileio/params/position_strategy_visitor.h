#pragma once
#include <functional>
#include <string>

#include "src/game_state/factory.h"

#include "entities_map.h"
#include "strategies.h"

import pln.scene.iscene;

class PositionStrategyVisitor
{
private:
  std::reference_wrapper<const EntityParamsMap> _entities;
  std::reference_wrapper<pln::scene::IScene> _scene;
  ModelFactory::MakerFn _maker_fn;

  PositionStrategyVisitor(
    std::reference_wrapper<const EntityParamsMap> entities,
    std::reference_wrapper<pln::scene::IScene> scene,
    ModelFactory::MakerFn maker_fn);

  [[nodiscard]] entt::entity handle_single(const std::string& entity_id);

public:
  PositionStrategyVisitor() = delete;

  void operator()(const PositionStrategyRound& strategy);
  void operator()(const PositionStrategySingle& strategy);
  void operator()(const PositionStrategyMany& strategy);
  void operator()(const PositionStrategySquare& strategy);
  void operator()(const PositionStrategyUndefined& strategy) const;
  void operator()(const PositionStrategyVoid& strategy);

  static void visit(const PositionStrategy& strategy,
                    const EntityParamsMap& entities,
                    pln::scene::IScene& scene,
                    const ModelFactory::MakerFn& maker_fn);
};
