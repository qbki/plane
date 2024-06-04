#include <gsl/pointers>
#include <stdexcept>
#include <string>
#include <utility>

#include "app_builder.h"

template<typename T>
T
or_throw(std::optional<T>& optional, const std::string& message)
{
  if (optional.has_value()) {
    return std::move(optional.value());
  }
  throw std::runtime_error("System is incomplete: " + message);
}

void
AppBuilder::control(std::unique_ptr<Control> control)
{
  _control = std::move(control);
}

void
AppBuilder::screen_size(int width, int height)
{
  _screen_size = std::make_unique<RectSize>(width, height);
}

void
AppBuilder::deferred_shading(std::unique_ptr<DeferredShading> deferred_shading)
{
  _deferred_shading = std::move(deferred_shading);
}

void
AppBuilder::particle_shader(std::unique_ptr<Shader> particle_shader)
{
  _particle_shader = std::move(particle_shader);
}

void
AppBuilder::window(WindowPtr window)
{
  _window = std::move(window);
}

void
AppBuilder::context(ContextPtr context)
{
  _context = std::move(context);
}

void
AppBuilder::theme(std::unique_ptr<GUI::Theme> theme)
{
  _theme = std::move(theme);
}

gsl::owner<App*>
AppBuilder::build()
{
  auto control_obj = or_throw(_control, "Control");
  auto screen_size_obj = or_throw(_screen_size, "Screen Size");
  auto deferred_shading_obj = or_throw(_deferred_shading, "Deferred Shading");
  auto particle_shader_obj = or_throw(_particle_shader, "Particle Shader");
  auto window_obj = or_throw(_window, "Window");
  auto context_obj = or_throw(_context, "Context");
  auto theme_obj = or_throw(_theme, "Theme");

  return new App(std::move(control_obj),
                 std::move(screen_size_obj),
                 std::move(deferred_shading_obj),
                 std::move(particle_shader_obj),
                 std::move(theme_obj),
                 std::move(window_obj),
                 std::move(context_obj));
}
