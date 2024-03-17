#pragma once
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

std::string
extract_kind(const nlohmann::basic_json<>& strategy_node,
             const nlohmann::basic_json<>& entities_node);
