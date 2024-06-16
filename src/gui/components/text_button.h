#pragma once
#include <entt/entt.hpp>
#include <memory>
#include <string>

namespace GUI::Factory {

entt::entity
make_text_button(std::shared_ptr<entt::registry>& registry,
                 const std::string& text);

}
