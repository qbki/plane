#include "loading_screen.h"

namespace GUI {

const int LOADING_TEXT_WIDTH = 450;
const int LOADING_TEXT_HEIGHT = 110;

LoadingScreen::LoadingScreen()
  : _text(std::make_unique<GUI::Text>(theme().typography.h1,
                                      LOADING_TEXT_WIDTH,
                                      LOADING_TEXT_HEIGHT,
                                      "Loading..."))
{
}

void
LoadingScreen::draw()
{
  const int offset = 16;
  auto screen_size = app().screen_size();
  auto text_pos = _text->position();
  glm::vec3 new_pos{
    screen_size.width - _text->width() - offset,
    offset,
    text_pos.z,
  };
  _text->position(new_pos);
  _text->draw();
}

}
