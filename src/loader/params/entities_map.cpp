#include <utility>

#include "entities_map.h"

void
EntityParamsMap::set(Mapping&& mapping)
{
  _mapping = std::move(mapping);
}

EntityParams
EntityParamsMap::params(const std::string& key) const
{
  if (_mapping.contains(key)) {
    return _mapping.at(key);
  }
  throw std::runtime_error(
    std::format(R"(Can't find entity params with key "{}")", key));
}

EntityParamsActor
EntityParamsMap::actor(const std::string& key) const
{
  return get<EntityParamsActor>(key);
}

EntityParamsModel
EntityParamsMap::model(const std::string& key) const
{
  return get<EntityParamsModel>(key);
}

EntityParamsDirectionalLight
EntityParamsMap::directional_light(const std::string& key) const
{
  return get<EntityParamsDirectionalLight>(key);
}

EntityParamsPointLight
EntityParamsMap::point_light(const std::string& key) const
{
  return get<EntityParamsPointLight>(key);
}

EntityParamsWeapon
EntityParamsMap::weapon(const std::string& key) const
{
  return get<EntityParamsWeapon>(key);
}

EntityParamsParticles
EntityParamsMap::particles(const std::string& key) const
{
  return get<EntityParamsParticles>(key);
}
