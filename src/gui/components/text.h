#pragma once
#include <entt/entt.hpp>
#include <string>

namespace GUI::Factory {

entt::entity
make_text(entt::registry& registry, const std::string& text);

}
