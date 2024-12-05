#pragma once
#include "src/app/app.h"                // IWYU pragma: export
#include "src/cache/cache.h"            // IWYU pragma: export
#include "src/events/event_emitter.h"   // IWYU pragma: export
#include "src/gui/core/theme.h"         // IWYU pragma: export
#include "src/logger/abstract_logger.h" // IWYU pragma: export
#include "src/mesh.h"
#include "src/utils/types.h"

#include "service.h"

namespace Services {

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

using GuiQuad = NewType<std::unique_ptr<Mesh>, struct GuiQuadTag>;
Mesh&
gui_quad();

template<typename T>
inline Events::EventEmitter<T>&
events()
{
  return Service<Events::EventEmitter<T>>::get();
}

}
