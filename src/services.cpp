#include "services.h"
#include "service.h"

namespace Services {

const GUI::Theme&
theme()
{
  return Service<const GUI::Theme>::get();
}

Mesh&
gui_quad()
{
  return *Service<const GuiQuad>::get().value;
}

}
