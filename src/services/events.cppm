export module pln.services.events;

import pln.events.event_emitter;
import pln.service;

namespace pln::services {

export
template<typename T>
inline pln::events::EventEmitter<T>&
events()
{
  return pln::Service<pln::events::EventEmitter<T>>::get();
}

}
