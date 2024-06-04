#pragma once
#include <glm/vec3.hpp>
#include <string>

#include "src/game_state/state.h"

#include "entities.h"

void
emit_particles(State& state,
               glm::vec3 initial_position,
               const EntityParamsParticles& params,
               const std::string& file_path);

void
emit_projectile(State& state,
                const entt::entity owner,
                const EntityParamsGun& params,
                const std::string& file_path);
