#pragma once
#include <memory>
#include <optional>

#include "control.h"
#include "game_state/game_state.h"
#include "sdl_init.h"
#include "shader.h"
#include "shading/deferred_shading.h"
#include "shapes.h"
#include "utils/types.h"

class App
{
public:
  struct Meta
  {
    App* app;
    float delta_time;
    float time;
  };

  unsigned long _last_time_point;

  std::unique_ptr<GameState> game_state;
  std::unique_ptr<Control> control;
  std::unique_ptr<RectSize> screen_size;
  std::unique_ptr<DeferredShading> deferred_shading;
  std::unique_ptr<Shader> particle_shader;
  WindowPtr window;
  ContextPtr gl_context;

  using Handler = std::function<void(Meta& meta)>;
  std::vector<Handler> _handlers = std::vector<Handler>();

  App(std::unique_ptr<GameState> _game_state,
      std::unique_ptr<Control> _control,
      std::unique_ptr<RectSize> _screen_size,
      std::unique_ptr<DeferredShading> _deferred_shading,
      std::unique_ptr<Shader> _particle_shader,
      WindowPtr _window,
      ContextPtr _gl_context);
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  App(App&&) = delete;
  App& operator=(App&&) = delete;
  ~App() = default;

  void add_handler(Handler handler);
  void update(unsigned long time_since_start_of_program);

  static glm::vec2 mouse_position();
};

class AppBuilder
{
public:
  OptionalPtr<GameState> _game_state;
  OptionalPtr<Control> _control;
  OptionalPtr<RectSize> _screen_size;
  OptionalPtr<DeferredShading> _deferred_shading;
  OptionalPtr<Shader> _particle_shader;
  std::optional<WindowPtr> _window;
  std::optional<ContextPtr> _context;

  AppBuilder& game_state(std::unique_ptr<GameState> game_state);
  AppBuilder& control(std::unique_ptr<Control> control);
  AppBuilder& screen_size(int width, int height);
  AppBuilder& deferred_shading(
    std::unique_ptr<DeferredShading> deferred_shading);
  AppBuilder& particle_shader(std::unique_ptr<Shader> particle_shader);
  AppBuilder& window(WindowPtr window);
  AppBuilder& context(ContextPtr context);

  App* build();
};
