#include <format>
#include <gsl/pointers>
#include <optional>
#include <string>
#include <utility>

#include "src/utils/crash.h"

#include "app_builder.h"

template<typename T>
T
or_crash(std::optional<T>& optional, const std::string& message)
{
  if (optional.has_value()) {
    return std::move(optional.value());
  }
  crash(std::format("System is incomplete: {}", message));
}

void
AppBuilder::control(std::unique_ptr<Control> value)
{
  _control = std::move(value);
}

void
AppBuilder::screen_size(int width, int height)
{
  _screen_size = { width, height };
}

void
AppBuilder::deferred_shading(std::unique_ptr<DeferredShading> value)
{
  _deferred_shading = std::move(value);
}

void
AppBuilder::particle_shader(std::unique_ptr<Shader> value)
{
  _particle_shader = std::move(value);
}

void
AppBuilder::window(WindowPtr value)
{
  _window = std::move(value);
}

void
AppBuilder::context(ContextPtr value)
{
  _context = std::move(value);
}

void
AppBuilder::intermediate_shader(std::unique_ptr<Shader> value)
{
  _intermediate_shader = std::move(value);
}

void
AppBuilder::assets_dir(std::filesystem::path value)
{
  _assets_dir = std::move(value);
}

gsl::owner<App*>
AppBuilder::build()
{
  auto control_obj = or_crash(_control, "Control");
  auto screen_size_obj = or_crash(_screen_size, "Screen Size");
  auto deferred_shading_obj = or_crash(_deferred_shading, "Deferred Shading");
  auto particle_shader_obj = or_crash(_particle_shader, "Particle Shader");
  auto intermediate_shader_obj = or_crash(_intermediate_shader,
                                          "Intermediate Shader");
  auto window_obj = or_crash(_window, "Window");
  auto context_obj = or_crash(_context, "Context");
  auto assets_dir_obj = or_crash(_assets_dir, "Assets directory");

  auto app = new App(std::move(control_obj),
                     screen_size_obj,
                     std::move(deferred_shading_obj),
                     std::move(particle_shader_obj),
                     std::move(intermediate_shader_obj),
                     std::move(window_obj),
                     std::move(context_obj),
                     std::move(assets_dir_obj));
  app->settings().load();
  return app;
}
