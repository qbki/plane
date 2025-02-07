#pragma once
#include <filesystem>
#include <glm/vec2.hpp> // IWYU pragma: export
#include <memory>
#include <optional>

#include "src/app/app.h" // IWYU pragma: export
#include "src/sdl_init.h"
#include "src/shader.h"
#include "src/utils/types.h"

class AppBuilder
{
public:
  OptionalPtr<Control> _control;
  std::optional<RectSize> _screen_size;
  OptionalPtr<DeferredShading> _deferred_shading;
  OptionalPtr<Shader> _particle_shader;
  OptionalPtr<Shader> _intermediate_shader;
  std::optional<WindowPtr> _window;
  std::optional<ContextPtr> _context;
  std::optional<std::filesystem::path> _assets_dir;

  void control(std::unique_ptr<Control> value);
  void screen_size(int width, int height);
  void deferred_shading(std::unique_ptr<DeferredShading> value);
  void particle_shader(std::unique_ptr<Shader> value);
  void intermediate_shader(std::unique_ptr<Shader> value);
  void window(WindowPtr value);
  void context(ContextPtr value);
  void assets_dir(std::filesystem::path value);

  App* build();
};
