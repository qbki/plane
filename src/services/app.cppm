export module pln.services.app;

import pln.app.app;
import pln.service;

namespace pln::services {

export
inline pln::app::App&
app()
{
  return pln::Service<pln::app::App>::get();
}

}
