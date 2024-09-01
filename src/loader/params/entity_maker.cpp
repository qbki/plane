#include <entt/entt.hpp>
#include <string>
#include <variant>

#include "entities_map.h"
#include "entity_maker.h"

const std::string PATH_NOT_DEFINED = "[path not defined]";

EntityMaker::EntityMaker(entt::registry* registry,
                         const EntityParamsMap* mapping,
                         const ModelFactory::MakerFn* maker_fn)
  : _mapping(mapping)
  , _registry(registry)
  , _maker_fn(maker_fn)
{
}

entt::entity
EntityMaker::operator()(const EntityParamsActor& params) const
{
  auto path = _mapping->model(params.model_id).path;
  return (*_maker_fn)(*_registry, path);
}

entt::entity
EntityMaker::operator()(const EntityParamsDirectionalLight&) const
{
  return (*_maker_fn)(*_registry, PATH_NOT_DEFINED);
}

entt::entity
EntityMaker::operator()(const EntityParamsWeapon& params) const
{
  auto path = _mapping->model(params.bullet_model_id).path;
  return (*_maker_fn)(*_registry, path);
}

entt::entity
EntityMaker::operator()(const EntityParamsModel& params) const
{
  auto path = params.path;
  return (*_maker_fn)(*_registry, path);
}

entt::entity
EntityMaker::operator()(const EntityParamsParticles& params) const
{
  auto path = _mapping->model(params.model_id).path;
  return (*_maker_fn)(*_registry, path);
}

entt::entity
EntityMaker::operator()(const EntityParamsPointLight&) const
{
  return (*_maker_fn)(*_registry, PATH_NOT_DEFINED);
}

entt::entity
EntityMaker::operator()(const EntityParamsTutorialButton& params) const
{
  auto path = _mapping->model(params.model_id).path;
  return (*_maker_fn)(*_registry, path);
}

entt::entity
EntityMaker::get(const EntityParams& params) const
{
  return std::visit(*this, params);
}
