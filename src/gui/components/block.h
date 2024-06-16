#pragma once
#include <entt/entt.hpp>
#include <memory>

namespace GUI::Factory {

entt::entity
make_block(std::shared_ptr<entt::registry>& registry);

}
