#include <algorithm>

#include "percent.h"
#include "src/utils/common.h"

Percent::Percent(double value)
  : _value(std::clamp(value, BOTTOM, TOP))
{
}

void
Percent::value(double value)
{
  auto new_value = std::clamp(value, BOTTOM, TOP);
  if (!is_approx_equal(new_value, _value)) {
    _value = new_value;
    _emitter.emit(_value);
  }
}

double
Percent::value() const
{
  return _value;
}

void
Percent::add(double term)
{
  value(_value + term);
}

Events::EventEmitter<double>&
Percent::on_changed()
{
  return _emitter;
}
