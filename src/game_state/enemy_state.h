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

  EnemyState(std::shared_ptr<Graphic> m) : model(m) {}
};
