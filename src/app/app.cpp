#include <SDL_timer.h>
#include <utility>

#include "app.h"

App::App(std::unique_ptr<Control> control,
         RectSize screen_size,
         std::unique_ptr<DeferredShading> deferred_shading,
         std::unique_ptr<Shader> particle_shader,
         std::unique_ptr<Shader> intermediate_shader,
         WindowPtr window,
         ContextPtr gl_context)
  : _last_time_point(SDL_GetTicks64())
  , _window(std::move(window))
  , _gl_context(std::move(gl_context))
  , _control(std::move(control))
  , _screen_size(screen_size)
  , _deferred_shading(std::move(deferred_shading))
  , _particle_shader(std::move(particle_shader))
  , _intermediate_shader(std::move(intermediate_shader))
{
}

Control&
App::control() const
{
  return *_control;
}

void
App::screen_size(const RectSize& size)
{
  _screen_size = size;
}

RectSize
App::screen_size() const
{
  return _screen_size;
}

DeferredShading&
App::deferred_shading() const
{
  return *_deferred_shading;
}

FrameBuffer&
App::intermediate_fb() const
{
  return *_intermediate_fb;
}

Shader&
App::particle_shader() const
{
  return *_particle_shader;
}

Shader&
App::intermediate_shader() const
{
  return *_intermediate_shader;
}

SDL_Window&
App::window() const
{
  return *_window;
}

SDL_GLContext
App::gl_context() const
{
  return _gl_context.get();
}

float
App::delta_time() const
{
  return _delta_time;
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

const System&
App::system() const
{
  return _system;
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
  const auto second_coeficient = 0.001f;
  _delta_time =
    static_cast<float>(time_since_start_of_program - _last_time_point) *
    second_coeficient;
  _time = static_cast<float>(_last_time_point) * second_coeficient;
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
