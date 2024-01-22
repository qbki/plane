#pragma once
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "src/app.h"

void
attach_velocity(const nlohmann::basic_json<>& node,
                entt::registry& registry,
                entt::entity entity);

void
attach_opaque(const nlohmann::basic_json<>& node,
              entt::registry& registry,
              entt::entity entity);

void
attach_position(const nlohmann::basic_json<>& node,
                entt::registry& registry,
                entt::entity entity);

void
attach_direction(const nlohmann::basic_json<>& node,
                 entt::registry& registry,
                 entt::entity entity);

void
attach_color(const nlohmann::basic_json<>& node,
             entt::registry& registry,
             entt::entity entity);

void
attach_particles_emmiter_by_hit(const nlohmann::basic_json<>& particle_node,
                                App& app,
                                entt::entity host_entity);

void
attach_projectile_emmiter(const nlohmann::basic_json<>& json_entities,
                          const nlohmann::basic_json<>& node,
                          App& app,
                          entt::entity host_entity);
