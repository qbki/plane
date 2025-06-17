export module pln.services.cache;

import pln.cache;
import pln.service;

namespace pln::services {

export
inline pln::cache::Cache&
cache()
{
  return pln::Service<pln::cache::Cache>::get();
}

}
