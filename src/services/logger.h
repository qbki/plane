#pragma once

import pln.logger;
import pln;

namespace Services {

inline pln::logger::AbstractLogger&
logger()
{
  return pln::Service<pln::logger::AbstractLogger>::get();
}

}
