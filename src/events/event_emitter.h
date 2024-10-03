#pragma once
#include <functional>
#include <map>
#include <ranges>
#include <unordered_set>

#include "event.h" // IWYU pragma: export

namespace Events {

using fn = void(void*);

template<typename... Ts>
class EventEmitter
{
public:
  using Handler = std::function<void(Ts&...)>;

private:
  unsigned int _current_event_id = 0;
  std::unordered_set<unsigned int> _should_be_called_once_ids {};
  std::map<unsigned int, Handler> _handlers {};

  void remove_by_id(unsigned int event_id)
  {
    if (_handlers.contains(event_id)) {
      _handlers.erase(event_id);
    }
    if (_should_be_called_once_ids.contains(event_id)) {
      _should_be_called_once_ids.erase(event_id);
    }
  }

  void clear_once_handlers()
  {
    for (auto id : _should_be_called_once_ids) {
      _handlers.erase(id);
    }
  }

public:
  void emit(Ts&... args)
  {
    for (const auto& handler : std::views::values(_handlers)) {
      handler(args...);
    }
    clear_once_handlers();
  }

  std::function<void()> add(const Handler& handler)
  {
    auto saved_event_id = _current_event_id;
    _handlers[saved_event_id] = handler;
    _current_event_id += 1;
    return [this, saved_event_id]() { remove_by_id(saved_event_id); };
  }

  std::function<void()> once(const Handler& handler)
  {
    _should_be_called_once_ids.insert(_current_event_id);
    return add(handler);
  }
};

}
