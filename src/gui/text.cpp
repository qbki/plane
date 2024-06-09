#include <SDL2/SDL_surface.h>
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <utility>

#include "src/consts.h"
#include "src/game_state/texture_type.h"
#include "src/gui/font.h"
#include "src/gui/theme.h"
#include "src/mesh.h"
#include "src/texture.h"

#include "text.h"

namespace GUI {

const std::unique_ptr<Mesh> Text::QUAD = nullptr;

std::vector<unsigned char>
get_pixels(int width, int height)
{
  std::vector<unsigned char> result;
  result.resize(width * height * 4, 0);
  return result;
}

std::vector<unsigned char>
Text::draw_text()
{
  auto result = get_pixels(_width, _height);
  if (_text.empty()) {
    return result;
  }
  auto font_surface = _font->draw(_text, GUI::Typography::DEFAULT_FONT_COLOR);
  auto draw_width = std::min(_width, font_surface->w);
  auto draw_height = std::min(_height, font_surface->h);
  auto draw_pitch = std::min(draw_width * 4, font_surface->pitch);
  auto src_pixels = static_cast<unsigned char*>(font_surface->pixels);
  for (int y = 0; y < draw_height; y++) {
    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    auto src_begin = src_pixels + y * font_surface->pitch;
    auto src_end = src_begin + draw_pitch;
    auto dst_begin = result.data() + (_height - y - 1) * _width * 4;
    std::copy(src_begin, src_end, dst_begin);
    // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }
  return result;
}

Text::Text(const std::shared_ptr<Font>& font,
           int width,
           int height,
           std::string text)
  : _font(font)
  , _text(std::move(text))
  , _is_dirty(true)
  , _width(width)
  , _height(height)
{
  if (!QUAD) {
    // The engine can create a Mesh only after OpenGL initialization.
    // So, There is no way to create QUAD as a normal constant.
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    const_cast<std::unique_ptr<Mesh>&>(QUAD) = Mesh::quad(HALF, HALF, 0.0);
  }
  auto pixels = get_pixels(_width, _height);
  _texture = std::make_unique<Texture>(_width, _height, pixels);
}

void
Text::draw()
{
  if (_is_dirty) {
    _texture->data(draw_text());
    _is_dirty = false;
  }
  auto transform =
    glm::translate(glm::mat4(1),
                   {
                     static_cast<float>(_width) * HALF + _position.x,
                     static_cast<float>(_height) * HALF + _position.y,
                     _position.z,
                   });
  transform = glm::scale(transform, { _width, _height, 1 });
  Mesh::DrawParams draw_params{
    .transforms{ transform },
    .texture_indices{ TextureType::Type::PRIMARY },
  };
  _texture->use(0);
  QUAD->draw(draw_params);
}

void
Text::text(const std::string& text)
{
  if (_text != text) {
    _text = text;
    _is_dirty = true;
  }
}

int
Text::width() const
{
  return _width;
}

int
Text::height() const
{
  return _height;
}

void
Text::position(const glm::vec3& value)
{
  _position = value;
}

glm::vec3
Text::position() const
{
  return _position;
}

}
