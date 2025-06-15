module;
#include "src/events/event_emitter.h"

export module pln.services.events;

import pln.service;

namespace pln::services {

export
template<typename T>
inline Events::EventEmitter<T>&
events()
{
  return pln::Service<Events::EventEmitter<T>>::get();
}

}
