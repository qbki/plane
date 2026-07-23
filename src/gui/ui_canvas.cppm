module;
#include <SDL3/SDL_surface.h>
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <thorvg.h>
#include <tuple>
#include <vector>

export module pln.gui.ui_canvas;

import pln.services.logger;
import pln.textures;

namespace pln::gui {

export
class UiCanvas
{
private:
  static constexpr tvg::SwCanvas::Colorspace COLOR_SPACE {
    tvg::SwCanvas::Colorspace::ARGB8888
  };

  int _width = 0;
  int _height = 0;
  std::vector<uint32_t> _target;
  std::unique_ptr<tvg::SwCanvas> _canvas;
  std::unique_ptr<pln::textures::Texture> _texture;

public:
  UiCanvas(const UiCanvas&) = delete;
  UiCanvas(UiCanvas&&) = default;
  UiCanvas& operator=(const UiCanvas&) = delete;
  UiCanvas& operator=(UiCanvas&&) = default;
  ~UiCanvas() = default;


  UiCanvas(int init_width, int init_height)
    : _width(init_width)
    , _height(init_height)
    , _canvas(tvg::SwCanvas::gen())
  {
    auto [norm_width, norm_height] = get_valid_size(_width, _height);
    _target.resize(norm_width * norm_height, 0);
    _canvas->target(
      _target.data(), norm_width, norm_width, norm_height, COLOR_SPACE);
    _texture = std::make_unique<pln::textures::Texture>(norm_width, norm_height, _target);
  }


  [[nodiscard]] int
  width() const
  {
    return _width;
  }


  [[nodiscard]] int
  height() const
  {
    return _height;
  }


  [[nodiscard]] const std::vector<uint32_t>&
  target() const
  {
    return _target;
  }


  tvg::Canvas&
  canvas()
  {
    return *_canvas;
  }


  pln::textures::Texture&
  texture()
  {
    return *_texture;
  }


  void
  draw()
  {
    if (_width <= 0 || _height <= 0) {
      return;
    }
    _canvas->draw();
    _canvas->sync();
    _texture->data(_target, _width, _height);
  }


  void
  clear()
  {
    _canvas->clear(true);
  }


  void
  save_to_file(const std::filesystem::path& path)
  {
    if (_width <= 0 || _height <= 0) {
      pln::services::logger().info(
        "Canvas has zero size, it can't be saved into a file");
      return;
    }

    auto sirface = SDL_CreateSurfaceFrom(_width,
                                         _height,
                                         SDL_PIXELFORMAT_RGBA8888,
                                         _target.data(),
                                         _width * 4);
    SDL_SaveBMP(sirface, path.c_str());
  }


private:
  std::tuple<int, int>
  get_valid_size(int width, int height)
  {
    return { std::max(width, 1), std::max(height, 1) };
  }
};

}
