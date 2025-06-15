module;
#include <memory>

#include "src/utils/types.h"

export module pln.services.gui_quad;

import pln.meshes;
import pln.service;

namespace pln::services {

export
using GuiQuad = NewType<std::unique_ptr<pln::meshes::Mesh>, struct GuiQuadTag>;

export
pln::meshes::Mesh&
gui_quad()
{
  return *pln::Service<const GuiQuad>::get().value;
}

}
