#include <format>
#include <memory>

#include "hostiles.h"
#include "text.h"
#include "theme.h"

namespace GUI {

Hostiles::Hostiles(const GUI::Theme& theme, int width, int height)
  : _text(std::make_unique<Text>(theme.typography.body1, width, height, ""))
{
}

void
Hostiles::quantity(int& value)
{
  _text->text(std::format("Hostiles {}", value));
}

void
Hostiles::draw()
{
  _text->draw();
}

}
