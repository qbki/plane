module;
#include "src/app/app.h"

export module pln.services.app;

import pln.service;

namespace pln::services {

export
inline App&
app()
{
  return pln::Service<App>::get();
}

}
