module;
#include <SDL_surface.h>
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
    constexpr int DEPTH = 32;
    constexpr uint32_t R_MASK = 0x00ff0000;
    constexpr uint32_t G_MASK = 0x0000ff00;
    constexpr uint32_t B_MASK = 0x000000ff;
    constexpr uint32_t A_MASK = 0xff000000;

    if (_width <= 0 || _height <= 0) {
      pln::services::logger().info(
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


private:
  std::tuple<int, int>
  get_valid_size(int width, int height)
  {
    return { std::max(width, 1), std::max(height, 1) };
  }
};

}
