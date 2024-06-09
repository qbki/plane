#pragma once

#include "src/gui/text.h"
#include "src/services.h"
#include <memory>

namespace GUI {

class LoadingScreen
{
private:
  std::unique_ptr<GUI::Text> _text{};

public:
  LoadingScreen();
  void draw();
};

}
