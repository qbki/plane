module;
#include "src/gui/core/theme.h"

export module pln.services.theme;

import pln.service;

namespace pln::services {

export
const GUI::Theme&
theme()
{
  return pln::Service<const GUI::Theme>::get();
}

}
