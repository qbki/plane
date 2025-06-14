#pragma once
#include "src/app/app.h"              // IWYU pragma: export
#include "src/cache/cache.h"          // IWYU pragma: export
#include "src/events/event_emitter.h" // IWYU pragma: export
#include "src/gui/core/theme.h"       // IWYU pragma: export
#include "src/utils/types.h"

import pln.meshes;
import pln;

namespace Services {

inline Cache&
cache()
{
  return pln::Service<Cache>::get();
}

inline App&
app()
{
  return pln::Service<App>::get();
}

const GUI::Theme&
theme();

using GuiQuad = NewType<std::unique_ptr<pln::meshes::Mesh>, struct GuiQuadTag>;
pln::meshes::Mesh&
gui_quad();

template<typename T>
inline Events::EventEmitter<T>&
events()
{
  return pln::Service<Events::EventEmitter<T>>::get();
}

}
