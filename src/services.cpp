#include "services.h"

import pln;

namespace Services {

const GUI::Theme&
theme()
{
  return pln::Service<const GUI::Theme>::get();
}

Mesh&
gui_quad()
{
  return *pln::Service<const GuiQuad>::get().value;
}

}
