#pragma once
#include "src/app.h"

#include "extractors.h"

struct ParticleParams
{
  unsigned int quantity{};
  ExtractVelocityResult velocity{};
  float lifetime{};
};

void
emit_particles(App& app,
               glm::vec3 initial_position,
               const ParticleParams& params,
               const std::string& file_path);

struct ProjectileParams
{
  ExtractVelocityResult velocity;
  float range;
};

void
emit_projectile(App& app,
                const ProjectileParams& params,
                const std::string& file_path);
