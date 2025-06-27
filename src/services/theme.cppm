export module pln.services.theme;

import pln.gui.core.theme;
import pln.service;

namespace pln::services {

export
const pln::gui::core::Theme&
theme()
{
  return pln::Service<const pln::gui::core::Theme>::get();
}

}
