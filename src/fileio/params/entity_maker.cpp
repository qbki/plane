#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <variant>

#include "entities_map.h"
#include "entity_maker.h"

const std::string PATH_NOT_DEFINED = "[path not defined]";

EntityMaker::EntityMaker(std::shared_ptr<entt::registry> registry,
                         const EntityParamsMap* mapping,
                         ModelFactory::MakerFn maker_fn)
  : _mapping(mapping)
  , _registry(std::move(registry))
  , _maker_fn(std::move(maker_fn))
{
}

entt::entity
EntityMaker::operator()(const EntityParamsActor& params)
{
  auto path = _mapping->model(params.model_id).path;
  return _maker_fn(_registry, path);
}

entt::entity
EntityMaker::operator()(const EntityParamsDirectionalLight&)
{
  return _maker_fn(_registry, PATH_NOT_DEFINED);
}

entt::entity
EntityMaker::operator()(const EntityParamsWeapon& params)
{
  auto path = _mapping->model(params.bullet_model_id).path;
  return _maker_fn(_registry, path);
}

entt::entity
EntityMaker::operator()(const EntityParamsModel& params)
{
  auto path = params.path;
  return _maker_fn(_registry, path);
}

entt::entity
EntityMaker::operator()(const EntityParamsParticles& params)
{
  auto path = _mapping->model(params.model_id).path;
  return _maker_fn(_registry, path);
}

entt::entity
EntityMaker::operator()(const EntityParamsPointLight&)
{
  return _maker_fn(_registry, PATH_NOT_DEFINED);
}

entt::entity
EntityMaker::operator()(const EntityParamsText&)
{
  return _maker_fn(_registry, PATH_NOT_DEFINED);
}

entt::entity
EntityMaker::operator()(const EntityParamsTutorialButton& params)
{
  auto path = _mapping->model(params.model_id).path;
  return _maker_fn(_registry, path);
}

entt::entity
EntityMaker::get(const EntityParams& params)
{
  return std::visit(*this, params);
}
