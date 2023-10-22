#include "particle_emitter.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <utility>


ParticleEmitter::ParticleEmitter(ModelEmitter emit)
  : _emit(emit) {}


void ParticleEmitter::emit(
  unsigned int quantity,
  glm::vec3 position,
  glm::vec3 direction
) {
  // TODO Calculate life time and scale of particle based on speed and damage of a projectile.
  float max_time = 0.2;
  auto rotation_step = glm::two_pi<double>() / static_cast<double>(quantity);
  auto setup_particle = [&](Model& model) {
    model.is_active(true);
    model.position(position);
    model.rotation_z(rotation_step * quantity);
  };

  for (auto& particle : _particles) {
    if (!particle.model->is_active()) {
      setup_particle(*particle.model);
      particle.max_live_time = max_time;
      particle.current_live_time = max_time;
      quantity--;
    }
    if (quantity <= 0) {
      break;
    }
  }

  while (quantity > 0) {
    auto model = this->_emit();
    setup_particle(*model);
    _particles.push_back({model, max_time, max_time});
    quantity--;
  }
}


void ParticleEmitter::draw() {
  for (auto& particle : _particles) {
    particle.model->draw();
  }
}


std::vector<ParticleEmitter::Particle>& ParticleEmitter::particles() {
  return _particles;
}
