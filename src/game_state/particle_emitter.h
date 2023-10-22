#pragma once
#include <memory>
#include <vector>
#include <functional>

#include "../models/index.h"


class ParticleEmitter {
private:
  struct Particle {
    std::shared_ptr<Model> model;
    float max_live_time;
    float current_live_time;
  };

  using ModelEmitter = std::function<std::shared_ptr<Model>()>;

  ModelEmitter _emit;
  std::vector<Particle> _particles;

public:
  ParticleEmitter(ModelEmitter emit);

  void emit(unsigned int quantity, glm::vec3 position, glm::vec3 direction);
  void draw();

  std::vector<Particle>& particles();
};
