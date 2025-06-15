export module pln.services.logger;

import pln.logger;
import pln.service;

namespace pln::services {

export
inline pln::logger::AbstractLogger&
logger()
{
  return pln::Service<pln::logger::AbstractLogger>::get();
}

}
