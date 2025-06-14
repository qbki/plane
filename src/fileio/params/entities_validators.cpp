#include <string>

#include "entities_validators.h"

import utils.crash;

void
validate(const EntityParamsText& value)
{
  if (value.size <= 0) {
    pln::utils::crash(R"("size" field of kind "text" must be 1 or more)");
  }

  if (value.width < 0.0f) {
    pln::utils::crash(R"("width" field of kind "text" must be more than zero)");
  }

  if (value.height < 0.0f) {
    pln::utils::crash(R"("height" field of kind "text" must be more than zero)");
  }
}
