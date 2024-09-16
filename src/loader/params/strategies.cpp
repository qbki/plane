#include <variant>

#include "src/utils/types.h"

#include "strategies.h"

BehaviourEnum
get_behaviour(const PositionStrategy& strategy)
{
  return std::visit(
    Overloaded {
      [](const auto& value) { return value.behaviour; },
      [](const PositionStrategyUndefined) { return BehaviourEnum::UNKNOWN; },
    },
    strategy);
}
