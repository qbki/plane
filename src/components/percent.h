#pragma once
#include "src/events/event_emitter.h"

class Percent
{
private:
  int _value = 0;
  Events::EventEmitter<int> _emitter;

public:
  constexpr static int TOP = 100;
  constexpr static int BOTTOM = 0;

  explicit Percent(int value);

  void value(int value);
  int value() const;

  void add(int term);

  double norm() const;

  Events::EventEmitter<int>& on_changed();
};
