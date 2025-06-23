module;
#include <glm/vec3.hpp>
#include <string>
#include <variant>
#include <vector>

#include "src/utils/types.h"

export module pln.fileio.params.strategies;

namespace pln::fileio::params {

export
enum class BehaviourEnum : char
{
  UNKNOWN = 0,
  ENEMY,
  LIGHT,
  PLAYER,
  STATIC,
  TUTORIAL_BUTTON,
};


export
struct PositionStrategyUndefined
{};


export
struct PositionStrategyRound
{
  BehaviourEnum behaviour;
  int radius;
  glm::vec3 center;
  std::vector<std::string> entity_ids;
};


export
struct PositionStrategySingle
{
  BehaviourEnum behaviour;
  glm::vec3 position;
  std::string entity_id;
};


export
struct PositionStrategyMany
{
  BehaviourEnum behaviour;
  std::vector<glm::vec3> positions;
  std::string entity_id;
};


export
struct PositionStrategySquare
{
  BehaviourEnum behaviour;
  glm::vec3 center;
  int width;
  int height;
  std::vector<std::string> entity_ids;
};


export
struct PositionStrategyVoid
{
  BehaviourEnum behaviour; // Supported only BehaviourEnum::LIGHT
  std::string entity_id;
};


export
using PositionStrategy = std::variant<PositionStrategyUndefined,
                                      PositionStrategyRound,
                                      PositionStrategySingle,
                                      PositionStrategyMany,
                                      PositionStrategySquare,
                                      PositionStrategyVoid>;


export
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

}
