#pragma once
#include <glm/vec3.hpp>
#include <string>

#include "src/app.h"
#include "src/loader/params.h"

void
emit_particles(App& app,
               glm::vec3 initial_position,
               const ParticlesParams& params,
               const std::string& file_path);

void
emit_projectile(App& app,
                const ProjectilesParams& params,
                const std::string& file_path);
