#pragma once
#include <entt/entt.hpp>

#include "src/game_state/factory.h"

#include "entities_map.h"

class EntityMaker
{
private:
  const EntityParamsMap* _mapping;
  entt::registry* _registry;
  const ModelFactory::MakerFn* _maker_fn;

public:
  EntityMaker(entt::registry* registry,
              const EntityParamsMap* mapping,
              const ModelFactory::MakerFn* maker_fn);
  entt::entity operator()(const EntityParamsActor& params) const;
  entt::entity operator()(const EntityParamsDirectionalLight&) const;
  entt::entity operator()(const EntityParamsGun& params) const;
  entt::entity operator()(const EntityParamsModel& params) const;
  entt::entity operator()(const EntityParamsParticles& params) const;
  entt::entity operator()(const EntityParamsPointLight&) const;
  entt::entity operator()(const EntityParamsTutorialButton& params) const;
  [[nodiscard]] entt::entity get(const EntityParams& params) const;
};
