#include <algorithm>
#include <memory>
#include <stdexcept>

#include "app.h"
#include "shader.h"
#include "shapes.h"
#include "utils.h"


App::App(
  std::unique_ptr<GameState> _game_state,
  std::unique_ptr<Control> _control,
  std::unique_ptr<RectSize> _screen_size,
  std::unique_ptr<DeferredShading> _deferred_shading,
  std::unique_ptr<Shader> _particle_shader,
  WindowPtr _window,
  ContextPtr _gl_context
) : game_state(std::move(_game_state)),
    control(std::move(_control)),
    screen_size(std::move(_screen_size)),
    deferred_shading(std::move(_deferred_shading)),
    particle_shader(std::move(_particle_shader)),
    window(std::move(_window)),
    gl_context(std::move(_gl_context))
{
  _last_time_point = SDL_GetTicks64();
}


template<typename T>
T or_throw(std::optional<T>& optional, const std::string& message) {
  if (optional.has_value()) {
    return std::move(optional.value());
  }
  throw std::runtime_error("System is incomplete: " + message);
}


AppBuilder& AppBuilder::game_state(std::unique_ptr<GameState> game_state) {
  _game_state = std::move(game_state);
  return *this;
}


AppBuilder& AppBuilder::control(std::unique_ptr<Control> control) {
  _control = std::move(control);
  return *this;
}


AppBuilder& AppBuilder::screen_size(int width, int height) {
  _screen_size = std::make_unique<RectSize>(width, height);
  return *this;
}


AppBuilder& AppBuilder::deferred_shading(std::unique_ptr<DeferredShading> deferred_shading) {
  _deferred_shading = std::move(deferred_shading);
  return *this;
}


AppBuilder& AppBuilder::particle_shader(std::unique_ptr<Shader> particle_shader) {
  _particle_shader = std::move(particle_shader);
  return *this;
}


AppBuilder& AppBuilder::window(WindowPtr window) {
  _window = std::move(window);
  return *this;
}


AppBuilder& AppBuilder::context(ContextPtr context) {
  _context = std::move(context);
  return *this;
}


App* AppBuilder::build() {
  auto game_state = or_throw(_game_state, "Game State");
  auto control = or_throw(_control, "Control");
  auto screen_size = or_throw(_screen_size, "Screen Size");
  auto deferred_shading = or_throw(_deferred_shading, "Deferred Shading");
  auto particle_shader = or_throw(_particle_shader, "Particle Shader");
  auto window = or_throw(_window, "Window");
  auto context = or_throw(_context, "Context");

  return new App(
    std::move(game_state),
    std::move(control),
    std::move(screen_size),
    std::move(deferred_shading),
    std::move(particle_shader),
    std::move(window),
    std::move(context)
  );
}


void App::add_handler(Handler handler) {
  _handlers.push_back(handler);
}


void App::update(App& app, unsigned long time_since_start_of_program) {
  Meta meta {
    .app = app,
    .delta_time = (time_since_start_of_program - _last_time_point) * 0.001f,
    .time = _last_time_point * 0.001f
  };
  _last_time_point = time_since_start_of_program;
  for (auto& handler : _handlers) {
    handler(meta);
  }
}


glm::vec2 App::mouse_position() {
  int x = 0;
  int y = 0;
  SDL_GetMouseState(&x, &y);
  return {x, y};
}
