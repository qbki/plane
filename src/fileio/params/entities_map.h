#pragma once
#include <algorithm>
#include <format>
#include <iterator>
#include <ranges>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "src/fileio/params/entities.h"
#include "src/utils/types.h"

import utils.crash;

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
    utils::crash(
      std::format(R"(Can't find an entity params with key "{}" and type "{}")",
                  key,
                  demangled_name<T>()));
  }

  template<typename T>
  std::vector<T> get_all() const
  {
    std::vector<T> result;
    auto data = _mapping | std::views::values
                | std::views::filter(
                  [](auto& value) { return std::holds_alternative<T>(value); })
                | std::views::transform([](auto& value) {
                    return std::get<EntityParamsModel>(value);
                  });
    std::ranges::copy(data, std::back_inserter(result));
    return result;
  }

  void set(Mapping&& mapping);
  [[nodiscard]] EntityParams params(const std::string& key) const;
  [[nodiscard]] EntityParamsActor actor(const std::string& key) const;
  [[nodiscard]] EntityParamsDirectionalLight directional_light(
    const std::string& key) const;
  [[nodiscard]] EntityParamsWeapon weapon(const std::string& key) const;
  [[nodiscard]] EntityParamsModel model(const std::string& key) const;
  [[nodiscard]] EntityParamsParticles particles(const std::string& key) const;
  [[nodiscard]] EntityParamsPointLight point_light(
    const std::string& key) const;
};
