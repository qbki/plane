#pragma once
#include "graphic.h"

class EnemyState {
public:
  enum State {
    HUNTING,
    SINKING,
  };

  State state = State::HUNTING;
  std::shared_ptr<Graphic> model;

  EnemyState(std::shared_ptr<Graphic> m) : model(m) {}
};
