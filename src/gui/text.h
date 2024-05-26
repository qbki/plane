#pragma once
#include <string>

#include "src/mesh.h"
#include "src/texture.h"

#include "font.h"

namespace GUI {

class Text
{
private:
  const static std::unique_ptr<Mesh> QUAD;

  std::unique_ptr<Texture> _texture;
  std::shared_ptr<Font> _font;
  std::string _text;
  bool _is_dirty;
  int _width;
  int _height;

  std::vector<unsigned char> draw_text();

public:
  Text(const std::shared_ptr<Font>& font,
       int width,
       int height,
       std::string text);
  void draw();
  void text(const std::string& text);
};

}
