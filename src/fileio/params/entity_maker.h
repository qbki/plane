#pragma once
#include <entt/entt.hpp>
#include <memory>

#include "src/game_state/factory.h"

#include "entities_map.h"

class EntityMaker
{
private:
  const EntityParamsMap* _mapping;
  std::shared_ptr<entt::registry> _registry;
  ModelFactory::MakerFn _maker_fn;

public:
  EntityMaker(std::shared_ptr<entt::registry> registry,
              const EntityParamsMap* mapping,
              ModelFactory::MakerFn maker_fn);
  entt::entity operator()(const EntityParamsActor& params);
  entt::entity operator()(const EntityParamsDirectionalLight&);
  entt::entity operator()(const EntityParamsModel& params);
  entt::entity operator()(const EntityParamsParticles& params);
  entt::entity operator()(const EntityParamsPointLight&);
  entt::entity operator()(const EntityParamsText& params);
  entt::entity operator()(const EntityParamsTutorialButton& params);
  entt::entity operator()(const EntityParamsWeapon& params);
  [[nodiscard]] entt::entity get(const EntityParams& params);
};
