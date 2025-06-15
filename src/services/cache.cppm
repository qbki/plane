module;
#include "src/cache/cache.h"

export module pln.services.cache;

import pln.service;

namespace pln::services {

export
inline Cache&
cache()
{
  return pln::Service<Cache>::get();
}

}
