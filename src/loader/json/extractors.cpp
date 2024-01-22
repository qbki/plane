#include "extractors.h"

glm::vec3
extract_vec3(const nlohmann::basic_json<>& node)
{
  return { node.at(0).get<float>(),
           node.at(1).get<float>(),
           node.at(2).get<float>() };
}

ExtractVelocityResult
extract_velocity(const nlohmann::basic_json<>& node)
{
  auto velocity_node = node.at("velocity");
  return { .acceleration = velocity_node.contains("acceleration")
                             ? velocity_node.at("acceleration").get<float>()
                             : 0,
           .speed = velocity_node.contains("speed")
                      ? velocity_node.at("speed").get<float>()
                      : 0,
           .damping = velocity_node.at("damping") };
}

std::string
extract_kind(const nlohmann::basic_json<>& strategy_node,
             const nlohmann::basic_json<>& entities_node)
{
  auto entity_id = strategy_node.at("entity_id").get<std::string>();
  auto entity_node = entities_node.at(entity_id);
  return entity_node.at("kind").get<std::string>();
}
