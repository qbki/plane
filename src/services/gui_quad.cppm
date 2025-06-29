module;
#include <memory>

export module pln.services.gui_quad;

import pln.mesh;
import pln.service;
import pln.utils.types;

namespace pln::services {

export
using GuiQuad = utils::NewType<std::unique_ptr<pln::mesh::Mesh>, struct GuiQuadTag>;

export
pln::mesh::Mesh&
gui_quad()
{
  return *pln::Service<const GuiQuad>::get().value;
}

}
