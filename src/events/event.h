#pragma once
#include <entt/entity/fwd.hpp>
#include <functional>
#include <string_view>
#include <variant>

namespace Events {

struct ShootEvent
{
  std::string_view sound_path;
};

struct LoadLevelEvent
{};

template<typename T>
using CB = std::function<void(const T&)>;

using Event = std::variant<ShootEvent>;

}
