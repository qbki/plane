#pragma once
#include <glm/vec2.hpp> // IWYU pragma: export
#include <memory>
#include <optional>

#include "src/app/app.h" // IWYU pragma: export
#include "src/gui/theme.h"
#include "src/sdl_init.h"
#include "src/utils/types.h"

class AppBuilder
{
public:
  OptionalPtr<Control> _control;
  OptionalPtr<RectSize> _screen_size;
  OptionalPtr<DeferredShading> _deferred_shading;
  OptionalPtr<Shader> _particle_shader;
  OptionalPtr<GUI::Theme> _theme;
  std::optional<WindowPtr> _window;
  std::optional<ContextPtr> _context;

  void control(std::unique_ptr<Control> control);
  void screen_size(int width, int height);
  void deferred_shading(std::unique_ptr<DeferredShading> deferred_shading);
  void particle_shader(std::unique_ptr<Shader> particle_shader);
  void window(WindowPtr window);
  void context(ContextPtr context);
  void theme(std::unique_ptr<GUI::Theme> theme);

  App* build();
};
