#pragma once
#include <functional>
#include <typeindex>
#include <vector>

#include "event.h" // IWYU pragma: export

namespace Events {

using fn = void(void*);

template<typename T>
class EventEmitter
{
public:
  using Handler = std::function<void(const T&)>;

private:
  std::vector<Handler> _handlers{};

public:
  void emit(const T& event) const
  {
    for (const auto& handler : _handlers) {
      handler(event);
    }
  }

  void add(Handler& handler) { _handlers.push_back(handler); }
};

}
