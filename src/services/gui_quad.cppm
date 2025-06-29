module;
#include <memory>

#include "src/utils/types.h"

export module pln.services.gui_quad;

import pln.mesh;
import pln.service;

namespace pln::services {

export
using GuiQuad = NewType<std::unique_ptr<pln::mesh::Mesh>, struct GuiQuadTag>;

export
pln::mesh::Mesh&
gui_quad()
{
  return *pln::Service<const GuiQuad>::get().value;
}

}
