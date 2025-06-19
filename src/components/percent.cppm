module;
#include <algorithm>

#include "src/events/event_emitter.h"

export module pln.components.percent;

namespace pln::components {

export
class Percent
{
private:
  int _value = 0;
  Events::EventEmitter<int> _emitter;

public:
  constexpr static int TOP = 100;
  constexpr static int BOTTOM = 0;

  Percent(int value)
    : _value(std::clamp(value, BOTTOM, TOP))
  {
  }


  void
  value(int value)
  {
    auto new_value = std::clamp(value, BOTTOM, TOP);
    if (new_value != _value) {
      _value = new_value;
      _emitter.emit(_value);
    }
  }


  int
  value() const
  {
    return _value;
  }


  void
  add(int term)
  {
    value(_value + term);
  }


  double
  norm() const
  {
    return static_cast<double>(_value) / static_cast<double>(TOP);
  }


  Events::EventEmitter<int>&
  on_changed()
  {
    return _emitter;
  }
};

}
