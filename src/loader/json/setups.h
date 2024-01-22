#pragma once
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "src/app.h"

void
setup_player(const nlohmann::basic_json<>& json_strategy,
             App& app,
             entt::entity entity);

void
setup_camera(const nlohmann::basic_json<>& json_level, App& app);
