#include <SDL_timer.h>
#include <filesystem> // IWYU pragma: export
#include <format>
#include <functional>
#include <utility>

#include "app.h"

import pln.app.save;
import pln.app.settings;
import pln.control;
import pln.sdl;
import pln.shaders;
import pln.shapes;
import pln.utils.crash;

App::App(std::filesystem::path assets_dir)
  : _last_time_point(SDL_GetTicks64())
  , _assets_dir(std::move(assets_dir))
{
}

template<typename T, typename F>
void
App::or_crash(std::unique_ptr<T, F>& ptr, const std::string& message)
{
  if (!ptr) {
    pln::utils::crash(std::format("System is incomplete: {}", message));
  }
}

void
App::control(std::unique_ptr<pln::control::Control> value)
{
  _control = std::move(value);
}

pln::control::Control&
App::control() const
{
  return *_control;
}

void
App::screen_size(const pln::shapes::RectSize& size)
{
  _screen_size = size;
}

pln::shapes::RectSize
App::screen_size() const
{
  return _screen_size;
}

void
App::deferred_shading(std::unique_ptr<DeferredShading> value)
{
  _deferred_shading = std::move(value);
}

DeferredShading&
App::deferred_shading() const
{
  return *_deferred_shading;
}

void
App::intermediate_fb(std::unique_ptr<FrameBuffer> value)
{
  _intermediate_fb = std::move(value);
}

FrameBuffer&
App::intermediate_fb() const
{
  return *_intermediate_fb;
}

void
App::particle_shader(std::unique_ptr<pln::shaders::Shader> value)
{
  _particle_shader = std::move(value);
}

pln::shaders::Shader&
App::particle_shader() const
{
  return *_particle_shader;
}

void
App::ui_shader(std::unique_ptr<pln::shaders::Shader> value)
{
  _ui_shader = std::move(value);
}

pln::shaders::Shader&
App::ui_shader() const
{
  return *_ui_shader;
}

void
App::intermediate_shader(std::unique_ptr<pln::shaders::Shader> value)
{
  _intermediate_shader = std::move(value);
}

pln::shaders::Shader&
App::intermediate_shader() const
{
  return *_intermediate_shader;
}

void
App::window(pln::sdl::WindowPtr value)
{
  _window = std::move(value);
}

SDL_Window&
App::window() const
{
  return *_window;
}

void
App::gl_context(pln::sdl::ContextPtr value)
{
  _gl_context = std::move(value);
}

SDL_GLContext
App::gl_context() const
{
  return _gl_context.get();
}

/**
 * Returns seconds since the last frame.
 */
float
App::delta_time() const
{
  return _delta_time;
}

std::filesystem::path
App::assets_dir() const
{
  return _assets_dir;
}

std::filesystem::path
App::levels_dir() const
{
  return _assets_dir / "levels";
}

std::filesystem::path
App::credits_file() const
{
  return _assets_dir / "text" / "credits.txt";
}

void
App::is_running(bool value)
{
  _is_game_running = value;
}

bool
App::is_running() const
{
  return _is_game_running;
}

void
App::push_scene(std::unique_ptr<Scene> scene)
{
  _scenes.push_back(std::move(scene));
}

void
App::pop_scene()
{
  if (!_scenes.empty()) {
    _scenes.pop_back();
  }
}

void
App::pause_scenes(bool value)
{
  for (auto& scene : _scenes) {
    scene->is_paused(value);
  }
}

std::vector<std::unique_ptr<Scene>>&
App::scenes()
{
  return _scenes;
}

AppInfo&
App::info()
{
  return _info;
}

pln::app::Settings&
App::settings()
{
  return _settings;
}

pln::app::Save&
App::save_data()
{
  return _save;
}

std::function<std::string(const std::string&)>&
App::translate_fn() const
{
  static std::function<std::string(const std::string&)> fn =
    [&](const std::string& key) {
      return _translations.contains(key) ? _translations.at(key) : key;
    };
  return fn;
}

void
App::set_translations(const TranslationsMapping& translations)
{
  _translations = translations;
}

void
App::add_handler(Handler handler)
{
  _handlers.push_back(handler);
}

void
App::add_once_handler(Handler handler)
{
  _once_handlers.push_back(handler);
}

void
App::update(unsigned long time_since_start_of_program)
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
App::validate()
{
  or_crash(_control, "Control");
  or_crash(_deferred_shading, "Deferred Shading");
  or_crash(_particle_shader, "Particle Shader");
  or_crash(_ui_shader, "UI Shader");
  or_crash(_intermediate_shader, "Intermediate Shader");
  or_crash(_window, "Window");
  or_crash(_gl_context, "GL Context");
}
