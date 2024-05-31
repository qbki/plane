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

using Event = std::variant<ShootEvent>;

}
