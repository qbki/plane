#include "extractors.h"

std::string
extract_kind(const nlohmann::basic_json<>& strategy_node,
             const nlohmann::basic_json<>& entities_node)
{
  auto entity_id = strategy_node.at("entity_id").get<std::string>();
  auto entity_node = entities_node.at(entity_id);
  return entity_node.at("kind").get<std::string>();
}
