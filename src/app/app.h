#pragma once
#include <functional>
#include <glm/vec2.hpp> // IWYU pragma: export
#include <memory>
#include <optional>
#include <vector>

#include "src/control.h"               // IWYU pragma: export
#include "src/game_state/game_state.h" // IWYU pragma: export
#include "src/gui/theme.h"
#include "src/sdl_init.h"
#include "src/shader.h"                   // IWYU pragma: export
#include "src/shading/deferred_shading.h" // IWYU pragma: export
#include "src/shapes.h"                   // IWYU pragma: export
#include "src/utils/types.h"

class App
{
public:
  using Handler = std::function<void(App& app)>;

private:
  unsigned long _last_time_point{};
  float _delta_time{};
  float _time{};
  WindowPtr _window;
  ContextPtr _gl_context;
  std::unique_ptr<GameState> _game_state;
  std::unique_ptr<Control> _control;
  std::unique_ptr<RectSize> _screen_size;
  std::unique_ptr<DeferredShading> _deferred_shading;
  std::unique_ptr<Shader> _particle_shader;
  std::unique_ptr<GUI::Theme> _theme;

  std::vector<Handler> _handlers = std::vector<Handler>();
  std::vector<Handler> _once_handlers = std::vector<Handler>();

public:
  App(std::unique_ptr<GameState> _game_state,
      std::unique_ptr<Control> _control,
      std::unique_ptr<RectSize> _screen_size,
      std::unique_ptr<DeferredShading> _deferred_shading,
      std::unique_ptr<Shader> _particle_shader,
      std::unique_ptr<GUI::Theme> _theme,
      WindowPtr _window,
      ContextPtr _gl_context);
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  App(App&&) = delete;
  App& operator=(App&&) = delete;
  ~App() = default;

  [[nodiscard]] GameState& game_state() const;
  [[nodiscard]] Control& control() const;
  [[nodiscard]] RectSize& screen_size() const;
  [[nodiscard]] DeferredShading& deferred_shading() const;
  [[nodiscard]] Shader& particle_shader() const;
  [[nodiscard]] GUI::Theme& theme() const;
  [[nodiscard]] SDL_Window& window() const;
  [[nodiscard]] SDL_GLContext gl_context() const;

  [[nodiscard]] float delta_time() const;

  void add_handler(Handler handler);
  void add_once_handler(Handler handler);
  void update(unsigned long time_since_start_of_program);

  static glm::vec2 mouse_position();
};
