#pragma once
#include "src/events/event_emitter.h"

class Percent
{
private:
  double _value = 0.0;
  Events::EventEmitter<double> _emitter;

public:
  constexpr static const double TOP = 1.0;
  constexpr static const double BOTTOM = 0.0;

  explicit Percent(double value);

  void value(double value);
  double value() const;

  void add(double term);

  Events::EventEmitter<double>& on_changed();
};
