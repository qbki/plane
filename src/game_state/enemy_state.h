#pragma once
#include "../models/index.h"

class EnemyState {
public:
  enum State {
    HUNTING,
    SINKING,
    INACTIVE,
  };

  State state = State::HUNTING;
  std::shared_ptr<Graphic> model;
  float hunting_speed = 1.0;

  EnemyState(std::shared_ptr<Graphic> m) : model(m) {}

  bool is_sibling_close(glm::vec3 sibling_position) {
    return glm::distance(this->model->position(), sibling_position) < 0.7;
  }
};
