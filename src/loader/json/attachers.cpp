#include <format>
#include <iterator>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/components/velocity.h"
#include "src/loader/params.h"

#include "attachers.h"
#include "emmiters.h"
#include "mappers.h" // IWYU pragma: keep

void
attach_velocity(const nlohmann::basic_json<>& node,
                entt::registry& registry,
                entt::entity entity)
{
  if (!node.contains("velocity")) {
    return;
  }
  auto velocity = node.at("velocity").get<VelocityParams>();
  registry.emplace_or_replace<Velocity>(
    entity, velocity.acceleration, velocity.damping);
}

void
attach_opaque(const nlohmann::basic_json<>& node,
              entt::registry& registry,
              entt::entity entity)
{
  if (!node.contains("is_opaque")) {
    return;
  }
  auto is_opaque = node.at("is_opaque").get<bool>();
  if (is_opaque) {
    registry.emplace_or_replace<Opaque>(entity);
  }
}

void
attach_transform(const nlohmann::basic_json<>& node,
                 entt::registry& registry,
                 entt::entity entity)
{
  Transform transform;
  bool is_set = false;
  if (node.contains("position")) {
    auto position = node.at("position").get<glm::vec3>();
    transform.translate(position);
    is_set = true;
  }
  if (node.contains("rotation")) {
    auto rotation = node.at("rotation").get<glm::vec3>();
    transform.rotate(rotation);
    is_set = true;
  }
  if (is_set) {
    registry.emplace_or_replace<Transform>(entity, transform);
  }
}

void
attach_direction(const nlohmann::basic_json<>& node,
                 entt::registry& registry,
                 entt::entity entity)
{
  if (!node.contains("direction")) {
    return;
  }
  auto direction = node.at("direction").get<glm::vec3>();
  registry.emplace_or_replace<Direction>(entity, direction);
}

void
attach_color(const nlohmann::basic_json<>& node,
             entt::registry& registry,
             entt::entity entity)
{
  if (!node.contains("color")) {
    return;
  }
  auto color = node.at("color").get<glm::vec3>();
  registry.emplace_or_replace<Color>(entity, color);
}

void
attach_tutorial_button_value(const nlohmann::basic_json<>& node,
                             entt::registry& registry,
                             entt::entity entity)
{
  if (!node.contains("button")) {
    return;
  }
  auto key_string = node.at("button").get<std::string>();
  std::unordered_map<std::string, Control::Action> mapping{
    { "left", Control::Action::LEFT },
    { "right", Control::Action::RIGHT },
    { "up", Control::Action::UP },
    { "down", Control::Action::DOWN },
    { "shooting", Control::Action::SHOOTING },
  };
  if (mapping.contains(key_string)) {
    registry.emplace_or_replace<TutorialButton>(entity, mapping.at(key_string));
  } else {
    auto keys = std::views::keys(mapping);
    auto possible_values = std::accumulate(
      begin(keys),
      end(keys),
      std::string{},
      [](const std::string& acc, const std::string& tail) -> std::string {
        return acc == "" ? std::format(R"("{}")", tail)
                         : std::format(R"({}, "{}")", acc, tail);
      });
    auto message = std::format(
      R"(An unknown value of "button" parameter. Found: "{}". Possible values: {})",
      key_string,
      possible_values);
    throw std::runtime_error(message);
  }
}

void
attach_particles_emmiter_by_hit(const nlohmann::basic_json<>& particle_node,
                                App& app,
                                entt::entity host_entity)
{
  auto& registry{ app.game_state->registry() };
  auto file_path{ particle_node.at("model").get<std::string>() };
  auto particle_params = particle_node.get<ParticlesParams>();
  ParticlesEmitter emitter{ [particle_params, file_path, &app](
                              glm::vec3 position) {
    emit_particles(app, position, particle_params, file_path);
  } };
  registry.emplace_or_replace<ParticlesEmitter>(host_entity, emitter);
}

void
attach_projectile_emmiter(const nlohmann::basic_json<>& json_entities,
                          const nlohmann::basic_json<>& node,
                          App& app,
                          entt::entity host_entity)
{
  if (!node.contains("gun_id")) {
    return;
  }
  auto gun_id = node.at("gun_id").get<std::string>();
  auto gun_node = json_entities.at(gun_id);
  auto& registry{ app.game_state->registry() };
  auto projectile_params = gun_node.get<ProjectilesParams>();
  auto projectile_node = json_entities.at(projectile_params.projectile_id);
  auto file_path = projectile_node.at("model").get<std::string>();
  ProjectileEmitter emitter{ [projectile_params, file_path, &app]() {
    emit_projectile(app, projectile_params, file_path);
  } };
  registry.emplace_or_replace<ProjectileEmitter>(host_entity, emitter);
}
