#pragma once
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

glm::vec3
extract_vec3(const nlohmann::basic_json<>& node);

struct ExtractVelocityResult
{
  float acceleration;
  float speed;
  float damping;
};

ExtractVelocityResult
extract_velocity(const nlohmann::basic_json<>& node);

template<typename T>
std::vector<T>
extract_vector(const nlohmann::basic_json<>& node)
{
  std::vector<T> result;
  std::ranges::copy(node | std::ranges::views::transform(
                             [](auto& v) { return v.template get<T>(); }),
                    std::back_inserter(result));
  return result;
}

std::string
extract_kind(const nlohmann::basic_json<>& strategy_node,
             const nlohmann::basic_json<>& entities_node);
