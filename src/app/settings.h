#pragma once
#include "src/components/percent.h"

class Settings
{
private:
  Percent _master_volume;

public:
  Settings();

  Percent& master_volume();
};
