#include "services.h"
#include <iostream>

const GUI::Theme&
theme()
{
  return Service<const GUI::Theme>::get();
}
