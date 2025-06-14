#pragma once

#include <filesystem> // IWYU pragma: export
#include <functional>
#include <glm/vec2.hpp> // IWYU pragma: export
#include <string>
#include <unordered_map>
#include <vector>

#include "src/app/app_info.h" // IWYU pragma: export
#include "src/app/save.h"     // IWYU pragma: export
#include "src/app/settings.h" // IWYU pragma: export
#include "src/app/system.h"   // IWYU pragma: export
#include "src/scene/scene.h" // IWYU pragma: export
#include "src/sdl_init.h"
#include "src/shader.h"                   // IWYU pragma: export
#include "src/shading/deferred_shading.h" // IWYU pragma: export
#include "src/shading/framebuffer.h"      // IWYU pragma: export
#include "src/shapes.h"                   // IWYU pragma: export

import pln.consts;
import pln.control;

class App
{
public:
  using Handler = std::function<void(App& app)>;
  using TranslationsMapping = std::unordered_map<std::string, std::string>;

private:
  unsigned long _last_time_point {};
  float _delta_time {};
  float _time {};
  bool _is_game_running = true;
  AppInfo _info {};
  WindowPtr _window { nullptr, [](SDL_Window*) {} };
  ContextPtr _gl_context { nullptr, [](SDL_GLContext) {} };
  System _system {};
  Settings _settings { pln::consts::SETTINGS_FILE };
  Save _save { pln::consts::SAVE_DATA_FILE };
  std::vector<std::unique_ptr<Scene>> _scenes {};
  std::unique_ptr<pln::control::Control> _control {};
  RectSize _screen_size { pln::consts::DEFAULT_SCREEN_WIDTH, pln::consts::DEFAULT_SCREEN_HEIGHT };
  std::unique_ptr<DeferredShading> _deferred_shading {};
  std::unique_ptr<FrameBuffer>
    _intermediate_fb = std::make_unique<FrameBuffer>();
  std::unique_ptr<Shader> _particle_shader {};
  std::unique_ptr<Shader> _ui_shader {};
  std::unique_ptr<Shader> _intermediate_shader {};
  TranslationsMapping _translations;
  std::filesystem::path _assets_dir { pln::consts::DEFAULT_ASSETS_DIR };

  std::vector<Handler> _handlers = std::vector<Handler>();
  std::vector<Handler> _once_handlers = std::vector<Handler>();

public:
  explicit App(std::filesystem::path assets_dir);
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  App(App&&) = delete;
  App& operator=(App&&) = delete;
  ~App() = default;

  void control(std::unique_ptr<pln::control::Control> value);
  [[nodiscard]] pln::control::Control& control() const;

  void deferred_shading(std::unique_ptr<DeferredShading> value);
  [[nodiscard]] DeferredShading& deferred_shading() const;

  void intermediate_fb(std::unique_ptr<FrameBuffer> value);
  [[nodiscard]] FrameBuffer& intermediate_fb() const;

  void particle_shader(std::unique_ptr<Shader> value);
  [[nodiscard]] Shader& particle_shader() const;

  void ui_shader(std::unique_ptr<Shader> value);
  [[nodiscard]] Shader& ui_shader() const;

  void intermediate_shader(std::unique_ptr<Shader> value);
  [[nodiscard]] Shader& intermediate_shader() const;

  void window(WindowPtr value);
  [[nodiscard]] SDL_Window& window() const;

  void gl_context(ContextPtr value);
  [[nodiscard]] SDL_GLContext gl_context() const;

  [[nodiscard]] Settings& settings();

  [[nodiscard]] Save& save_data();

  [[nodiscard]] std::function<std::string(const std::string&)>& translate_fn()
    const;
  void set_translations(const TranslationsMapping& translations);

  [[nodiscard]] float delta_time() const;
  [[nodiscard]] std::filesystem::path assets_dir() const;
  [[nodiscard]] std::filesystem::path levels_dir() const;
  [[nodiscard]] std::filesystem::path credits_file() const;

  void is_running(bool value);
  [[nodiscard]] bool is_running() const;

  void push_scene(std::unique_ptr<Scene> scene);
  void pop_scene();
  void pause_scenes(bool value = true);
  [[nodiscard]] std::vector<std::unique_ptr<Scene>>& scenes();

  [[nodiscard]] AppInfo& info();
  [[nodiscard]] const System& system() const;

  void add_handler(Handler handler);
  void add_once_handler(Handler handler);
  void update(unsigned long time_since_start_of_program);

  void screen_size(const RectSize& size);
  [[nodiscard]] RectSize screen_size() const;

  void validate();

private:
  template<typename T, typename F>
  static void or_crash(std::unique_ptr<T, F>& ptr, const std::string& message);
};
