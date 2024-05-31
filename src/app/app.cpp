#include <SDL_mouse.h>
#include <SDL_timer.h>
#include <algorithm>
#include <gsl/pointers>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "app.h"

App::App(std::unique_ptr<GameState> _game_state,
         std::unique_ptr<Control> _control,
         std::unique_ptr<RectSize> _screen_size,
         std::unique_ptr<DeferredShading> _deferred_shading,
         std::unique_ptr<Shader> _particle_shader,
         std::unique_ptr<GUI::Theme> _theme,
         WindowPtr _window,
         ContextPtr _gl_context)
  : _last_time_point(SDL_GetTicks64())
  , _window(std::move(_window))
  , _gl_context(std::move(_gl_context))
  , _game_state(std::move(_game_state))
  , _control(std::move(_control))
  , _screen_size(std::move(_screen_size))
  , _deferred_shading(std::move(_deferred_shading))
  , _particle_shader(std::move(_particle_shader))
  , _theme(std::move(_theme))
{
}

GameState&
App::game_state() const
{
  return *_game_state;
}

Control&
App::control() const
{
  return *_control;
}

RectSize&
App::screen_size() const
{
  return *_screen_size;
}

DeferredShading&
App::deferred_shading() const
{
  return *_deferred_shading;
}

Shader&
App::particle_shader() const
{
  return *_particle_shader;
}

GUI::Theme&
App::theme() const
{
  return *_theme;
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

glm::vec2
App::mouse_position()
{
  int x = 0;
  int y = 0;
  SDL_GetMouseState(&x, &y);
  return { x, y };
}
