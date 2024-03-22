#pragma once
#include <nlohmann/json.hpp>
#include <string>

#include "src/app.h"
#include "src/game_state/factory.h"
#include "src/loader/params.h"

class PositionStrategyVisitor
{
private:
  const nlohmann::basic_json<>* _json_entities;
  const nlohmann::basic_json<>* _json_strategy;
  App* _app;
  ModelFactory::MakerFn* _maker_fn;

  entt::entity handle_single(const std::string& entity_id);

public:
  PositionStrategyVisitor(const nlohmann::basic_json<>* json_entities,
                          const nlohmann::basic_json<>* json_strategy,
                          App* app,
                          ModelFactory::MakerFn* maker_fn);
  void operator()(const PositionStrategyRound& startegy);
  void operator()(const PositionStrategySingle& startegy);
  void operator()(const PositionStrategySquare& startegy);
  void operator()(const PositionStrategyUndefined& strategy);
  void operator()(const PositionStrategyVoid& strategy);
};
