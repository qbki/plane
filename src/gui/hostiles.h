#pragma once
#include <memory>

#include "text.h"
#include "theme.h"

namespace GUI {

class Hostiles
{
private:
  std::unique_ptr<Text> _text;

public:
  Hostiles(const GUI::Theme& theme, int width, int height);
  void quantity(int& value);
  void draw();
};

}
