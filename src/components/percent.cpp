#include <algorithm>

#include "percent.h"

Percent::Percent(int value)
  : _value(std::clamp(value, BOTTOM, TOP))
{
}

void
Percent::value(int value)
{
  auto new_value = std::clamp(value, BOTTOM, TOP);
  if (new_value != _value) {
    _value = new_value;
    _emitter.emit(_value);
  }
}

int
Percent::value() const
{
  return _value;
}

void
Percent::add(int term)
{
  value(_value + term);
}

double
Percent::norm() const
{
  return static_cast<double>(_value) / static_cast<double>(TOP);
}

Events::EventEmitter<int>&
Percent::on_changed()
{
  return _emitter;
}
