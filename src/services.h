#pragma once
#include "src/app/app.h"              // IWYU pragma: export
#include "src/cache/cache.h"          // IWYU pragma: export
#include "src/events/event_emitter.h" // IWYU pragma: export
#include "src/gui/theme.h"
#include "src/logger/abstract_logger.h" // IWYU pragma: export

#include "service.h"

inline AbstractLogger&
logger()
{
  return Service<AbstractLogger>::get();
}

inline Cache&
cache()
{
  return Service<Cache>::get();
}

inline App&
app()
{
  return Service<App>::get();
}

const GUI::Theme&
theme();

template<typename T>
inline Events::EventEmitter<T>&
events()
{
  return Service<Events::EventEmitter<T>>::get();
}
