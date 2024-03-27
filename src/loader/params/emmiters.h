#pragma once
#include <glm/vec3.hpp>
#include <string>

#include "src/app.h"

#include "entities.h"

void
emit_particles(App& app,
               glm::vec3 initial_position,
               const EntityParamsParticles& params,
               const std::string& file_path);

void
emit_projectile(App& app,
                const EntityParamsGun& params,
                const std::string& file_path);
