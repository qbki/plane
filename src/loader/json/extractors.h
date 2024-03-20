#pragma once
#include <nlohmann/json.hpp>
#include <string>

std::string
extract_kind(const nlohmann::basic_json<>& strategy_node,
             const nlohmann::basic_json<>& entities_node);
