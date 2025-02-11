#pragma once
#include <functional>
#include <string>

#include "src/game_state/factory.h"
#include "src/scene/scene.h"

#include "entities_map.h"
#include "strategies.h"

class PositionStrategyVisitor
{
private:
  std::reference_wrapper<const EntityParamsMap> _entities;
  std::reference_wrapper<Scene> _scene;
  ModelFactory::MakerFn _maker_fn;

  PositionStrategyVisitor(
    std::reference_wrapper<const EntityParamsMap> entities,
    std::reference_wrapper<Scene> scene,
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
                    Scene& scene,
                    const ModelFactory::MakerFn& maker_fn);
};
