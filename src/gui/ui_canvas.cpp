#include <SDL_surface.h>
#include <algorithm>
#include <string>

#include "src/services/logger.h"
#include "src/texture.h"

#include "ui_canvas.h"

UiCanvas::UiCanvas(int init_width, int init_height)
  : _width(init_width)
  , _height(init_height)
  , _canvas(tvg::SwCanvas::gen())
{
  auto [norm_width, norm_height] = get_valid_size(_width, _height);
  _target.resize(norm_width * norm_height, 0);
  _canvas->target(
    _target.data(), norm_width, norm_width, norm_height, COLOR_SPACE);
  _texture = std::make_unique<Texture>(norm_width, norm_height, _target);
}

int
UiCanvas::width() const
{
  return _width;
}

int
UiCanvas::height() const
{
  return _height;
}

const std::vector<uint32_t>&
UiCanvas::target() const
{
  return _target;
}

tvg::Canvas&
UiCanvas::canvas()
{
  return *_canvas;
}

Texture&
UiCanvas::texture()
{
  return *_texture;
}

void
UiCanvas::draw()
{
  if (_width <= 0 || _height <= 0) {
    return;
  }
  _canvas->draw();
  _canvas->sync();
  _texture->data(_target, _width, _height);
}

void
UiCanvas::clear()
{
  _canvas->clear(true);
}

void
UiCanvas::save_to_file(const std::filesystem::path& path)
{
  constexpr int DEPTH = 32;
  constexpr uint32_t R_MASK = 0x00ff0000;
  constexpr uint32_t G_MASK = 0x0000ff00;
  constexpr uint32_t B_MASK = 0x000000ff;
  constexpr uint32_t A_MASK = 0xff000000;

  if (_width <= 0 || _height <= 0) {
    Services::logger().info(
      "Canvas has zero size, it can't be saved into a file");
    return;
  }

  auto sirface = SDL_CreateRGBSurfaceFrom(_target.data(),
                                          _width,
                                          _height,
                                          DEPTH,
                                          _width * 4,
                                          R_MASK,
                                          G_MASK,
                                          B_MASK,
                                          A_MASK);
  SDL_SaveBMP(sirface, path.c_str());
}

std::tuple<int, int>
UiCanvas::get_valid_size(int width, int height)
{
  return { std::max(width, 1), std::max(height, 1) };
}
