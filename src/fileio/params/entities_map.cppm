module;
#include <algorithm>
#include <format>
#include <iterator>
#include <ranges>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "src/utils/types.h"

export module pln.fileio.params.entities_map;

import pln.fileio.params.entities;
import pln.utils.crash;

namespace pln::fileio::params {

export
class EntityParamsMap
{
public:
  using Mapping = std::unordered_map<std::string, EntityParams>;

private:
  Mapping _mapping;

public:
  template<typename T>
  T get(const std::string& key) const
  {
    auto value = _mapping.at(key);
    if (std::holds_alternative<T>(value)) {
      return std::get<T>(value);
    }
    pln::utils::crash(
      std::format(R"(Can't find an entity params with key "{}" and type "{}")",
                  key,
                  demangled_name<T>()));
  }

  template<typename T>
  std::vector<T> get_all() const
  {
    auto data = _mapping | std::views::values;

    std::vector<EntityParams> filterd_data {};
    copy_if(data.begin(),
            data.end(),
            std::back_inserter(filterd_data),
            [](const EntityParams& value) {
              return std::holds_alternative<T>(value);
            });

    std::vector<T> transformed_data {};
    std::transform(filterd_data.begin(),
                   filterd_data.end(),
                   std::back_inserter(transformed_data),
                   [](const EntityParams& value) {
                     return std::get<T>(value);
                   });

    return transformed_data;
  }


  void
  set(Mapping&& mapping)
  {
    _mapping = std::move(mapping);
  }


  EntityParams
  params(const std::string& key) const
  {
    if (_mapping.contains(key)) {
      return _mapping.at(key);
    }
    pln::utils::crash(std::format(R"(Can't find entity params with key "{}")", key));
  }


  EntityParamsActor
  actor(const std::string& key) const
  {
    return get<EntityParamsActor>(key);
  }


  EntityParamsModel
  model(const std::string& key) const
  {
    return get<EntityParamsModel>(key);
  }


  EntityParamsDirectionalLight
  directional_light(const std::string& key) const
  {
    return get<EntityParamsDirectionalLight>(key);
  }

  EntityParamsPointLight
  point_light(const std::string& key) const
  {
    return get<EntityParamsPointLight>(key);
  }

  EntityParamsWeapon
  weapon(const std::string& key) const
  {
    return get<EntityParamsWeapon>(key);
  }

  EntityParamsParticles
  particles(const std::string& key) const
  {
    return get<EntityParamsParticles>(key);
  }
};

}
