module;
#include <SDL_timer.h>
#include <SDL_video.h>
#include <filesystem> // IWYU pragma: export
#include <format>
#include <functional>
#include <glm/vec2.hpp> // IWYU pragma: export
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "src/shading/deferred_shading.h" // IWYU pragma: export
#include "src/shading/framebuffer.h"      // IWYU pragma: export

export module pln.app.app;

import pln.app.info;
import pln.app.save;
import pln.app.settings;
import pln.consts;
import pln.control;
import pln.math.shapes;
import pln.scene.iscene;
import pln.sdl;
import pln.shaders;
import pln.utils.crash;

namespace pln::app {

export class App
{
public:
  using Handler = std::function<void(App& app)>;
  using TranslationsMapping = std::unordered_map<std::string, std::string>;

private:
  unsigned long _last_time_point {};
  float _delta_time {};
  float _time {};
  bool _is_game_running = true;
  pln::app::Info _info {};
  pln::sdl::WindowPtr _window { nullptr, [](SDL_Window*) {} };
  pln::sdl::ContextPtr _gl_context { nullptr, [](SDL_GLContext) {} };
  pln::app::Settings _settings { pln::consts::SETTINGS_FILE };
  pln::app::Save _save { pln::consts::SAVE_DATA_FILE };
  std::vector<std::unique_ptr<pln::scene::IScene>> _scenes {};
  std::unique_ptr<pln::control::Control> _control {};
  pln::math::RectSize _screen_size { pln::consts::DEFAULT_SCREEN_WIDTH,
                                     pln::consts::DEFAULT_SCREEN_HEIGHT };
  std::unique_ptr<DeferredShading> _deferred_shading {};
  std::unique_ptr<FrameBuffer>
    _intermediate_fb = std::make_unique<FrameBuffer>();
  std::unique_ptr<pln::shaders::Shader> _particle_shader {};
  std::unique_ptr<pln::shaders::Shader> _ui_shader {};
  std::unique_ptr<pln::shaders::Shader> _intermediate_shader {};
  TranslationsMapping _translations;
  std::filesystem::path _assets_dir { pln::consts::DEFAULT_ASSETS_DIR };

  std::vector<Handler> _handlers = std::vector<Handler>();
  std::vector<Handler> _once_handlers = std::vector<Handler>();

public:
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  App(App&&) = delete;
  App& operator=(App&&) = delete;
  ~App() = default;

  App(std::filesystem::path assets_dir)
    : _last_time_point(SDL_GetTicks64())
    , _assets_dir(std::move(assets_dir))
  {
  }

  void
  control(std::unique_ptr<pln::control::Control> value)
  {
    _control = std::move(value);
  }

  pln::control::Control&
  control() const
  {
    return *_control;
  }

  void
  screen_size(const pln::math::RectSize& size)
  {
    _screen_size = size;
  }

  pln::math::RectSize
  screen_size() const
  {
    return _screen_size;
  }

  void
  deferred_shading(std::unique_ptr<DeferredShading> value)
  {
    _deferred_shading = std::move(value);
  }

  DeferredShading&
  deferred_shading() const
  {
    return *_deferred_shading;
  }

  void
  intermediate_fb(std::unique_ptr<FrameBuffer> value)
  {
    _intermediate_fb = std::move(value);
  }

  FrameBuffer&
  intermediate_fb() const
  {
    return *_intermediate_fb;
  }

  void
  particle_shader(std::unique_ptr<pln::shaders::Shader> value)
  {
    _particle_shader = std::move(value);
  }

  pln::shaders::Shader&
  particle_shader() const
  {
    return *_particle_shader;
  }

  void
  ui_shader(std::unique_ptr<pln::shaders::Shader> value)
  {
    _ui_shader = std::move(value);
  }

  pln::shaders::Shader&
  ui_shader() const
  {
    return *_ui_shader;
  }

  void
  intermediate_shader(std::unique_ptr<pln::shaders::Shader> value)
  {
    _intermediate_shader = std::move(value);
  }

  pln::shaders::Shader&
  intermediate_shader() const
  {
    return *_intermediate_shader;
  }

  void
  window(pln::sdl::WindowPtr value)
  {
    _window = std::move(value);
  }

  SDL_Window&
  window() const
  {
    return *_window;
  }

  void
  gl_context(pln::sdl::ContextPtr value)
  {
    _gl_context = std::move(value);
  }

  SDL_GLContext
  gl_context() const
  {
    return _gl_context.get();
  }

  /**
   * Returns seconds since the last frame.
   */
  float
  delta_time() const
  {
    return _delta_time;
  }

  std::filesystem::path
  assets_dir() const
  {
    return _assets_dir;
  }

  std::filesystem::path
  levels_dir() const
  {
    return _assets_dir / "levels";
  }

  std::filesystem::path
  credits_file() const
  {
    return _assets_dir / "text" / "credits.txt";
  }

  void
  is_running(bool value)
  {
    _is_game_running = value;
  }

  bool
  is_running() const
  {
    return _is_game_running;
  }

  void
  push_scene(std::unique_ptr<pln::scene::IScene> scene)
  {
    _scenes.push_back(std::move(scene));
  }

  void
  pop_scene()
  {
    if (!_scenes.empty()) {
      _scenes.pop_back();
    }
  }

  void
  pause_scenes(bool value = true)
  {
    for (auto& scene : _scenes) {
      scene->is_paused(value);
    }
  }

  std::vector<std::unique_ptr<pln::scene::IScene>>&
  scenes()
  {
    return _scenes;
  }

  pln::app::Info&
  info()
  {
    return _info;
  }

  pln::app::Settings&
  settings()
  {
    return _settings;
  }

  pln::app::Save&
  save_data()
  {
    return _save;
  }

  std::function<std::string(const std::string&)>&
  translate_fn() const
  {
    static std::function<std::string(const std::string&)> fn =
      [&](const std::string& key) {
        return _translations.contains(key) ? _translations.at(key) : key;
      };
    return fn;
  }

  void
  set_translations(const TranslationsMapping& translations)
  {
    _translations = translations;
  }

  void
  add_handler(Handler handler)
  {
    _handlers.push_back(handler);
  }

  void
  add_once_handler(Handler handler)
  {
    _once_handlers.push_back(handler);
  }


  void
  update(unsigned long time_since_start_of_program)
  {
    const auto second_coefficient = 0.001f;
    _delta_time = static_cast<float>(time_since_start_of_program
                                     - _last_time_point)
                  * second_coefficient;
    _time = static_cast<float>(_last_time_point) * second_coefficient;
    _last_time_point = time_since_start_of_program;
    for (const auto& handler : _once_handlers) {
      handler(*this);
    }
    if (!_once_handlers.empty()) {
      _once_handlers.clear();
    }
    for (const auto& handler : _handlers) {
      handler(*this);
    }
  }


  void
  validate()
  {
    or_crash(_control, "Control");
    or_crash(_deferred_shading, "Deferred Shading");
    or_crash(_particle_shader, "Particle Shader");
    or_crash(_ui_shader, "UI Shader");
    or_crash(_intermediate_shader, "Intermediate Shader");
    or_crash(_window, "Window");
    or_crash(_gl_context, "GL Context");
  }

private:
  template<typename T, typename F>
  void
  or_crash(std::unique_ptr<T, F>& ptr, const std::string& message)
  {
    if (!ptr) {
      pln::utils::crash(std::format("System is incomplete: {}", message));
    }
  }
};

}
