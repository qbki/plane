module;
#include <entt/entt.hpp>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <variant>

export module pln.fileio.params.entity_maker;

import pln.fileio.params.entities;
import pln.fileio.params.entities_map;
import pln.fileio.params.strategies;
import pln.game_state.factory;

using namespace pln::fileio::params;
using namespace pln::game_state;

namespace pln::fileio::params {

static const std::string PATH_NOT_DEFINED = "[path not defined]";

export
class EntityMaker
{
private:
  std::reference_wrapper<const EntityParamsMap> _mapping;
  std::shared_ptr<entt::registry> _registry;
  factory::MakerFn _maker_fn;

  EntityMaker(std::shared_ptr<entt::registry> registry,
              std::reference_wrapper<const EntityParamsMap> mapping,
              factory::MakerFn maker_fn)
    : _mapping(mapping)
    , _registry(std::move(registry))
    , _maker_fn(std::move(maker_fn))
  {
  }

public:
  entt::entity
  operator()(const EntityParamsActor& params)
  {
    auto path = _mapping.get().model(params.model_id).path;
    return _maker_fn(_registry, path);
  }

  entt::entity
  operator()(const EntityParamsDirectionalLight&)
  {
    return _maker_fn(_registry, PATH_NOT_DEFINED);
  }

  entt::entity
  operator()(const EntityParamsWeapon& params)
  {
    auto path = _mapping.get().model(params.bullet_model_id).path;
    return _maker_fn(_registry, path);
  }

  entt::entity
  operator()(const EntityParamsModel& params)
  {
    auto path = params.path;
    return _maker_fn(_registry, path);
  }

  entt::entity
  operator()(const EntityParamsParticles& params)
  {
    auto path = _mapping.get().model(params.model_id).path;
    return _maker_fn(_registry, path);
  }

  entt::entity
  operator()(const EntityParamsPointLight&)
  {
    return _maker_fn(_registry, PATH_NOT_DEFINED);
  }

  entt::entity
  operator()(const EntityParamsText&)
  {
    return _maker_fn(_registry, PATH_NOT_DEFINED);
  }

  entt::entity
  operator()(const EntityParamsTutorialButton& params)
  {
    auto path = _mapping.get().model(params.model_id).path;
    return _maker_fn(_registry, path);
  }

  entt::entity
  get(const EntityParams& params)
  {
    return std::visit(*this, params);
  }

  [[nodiscard]] entt::entity
  static get(const std::shared_ptr<entt::registry>& registry,
      const EntityParamsMap& mapping,
      const factory::MakerFn& maker_fn,
      const EntityParams& params)
  {
    return EntityMaker(registry, mapping, maker_fn).get(params);
  }
};

}
