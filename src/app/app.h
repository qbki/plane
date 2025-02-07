#pragma once
#include <filesystem>
#include <functional>
#include <glm/vec2.hpp> // IWYU pragma: export
#include <string>
#include <unordered_map>
#include <vector>

#include "src/app/app_info.h" // IWYU pragma: export
#include "src/app/save.h"     // IWYU pragma: export
#include "src/app/settings.h" // IWYU pragma: export
#include "src/app/system.h"   // IWYU pragma: export
#include "src/consts.h"
#include "src/control.h" // IWYU pragma: export
#include "src/scene/scene.h"
#include "src/sdl_init.h"
#include "src/shader.h"                   // IWYU pragma: export
#include "src/shading/deferred_shading.h" // IWYU pragma: export
#include "src/shading/framebuffer.h"      // IWYU pragma: export
#include "src/shapes.h"                   // IWYU pragma: export

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
  WindowPtr _window;
  ContextPtr _gl_context;
  System _system {};
  Settings _settings { SETTINGS_FILE };
  Save _save { SAVE_DATA_FILE };
  std::vector<std::unique_ptr<Scene>> _scenes {};
  std::unique_ptr<Control> _control;
  RectSize _screen_size;
  std::unique_ptr<DeferredShading> _deferred_shading;
  std::unique_ptr<FrameBuffer>
    _intermediate_fb = std::make_unique<FrameBuffer>();
  std::unique_ptr<Shader> _particle_shader;
  std::unique_ptr<Shader> _intermediate_shader;
  TranslationsMapping _translations;
  std::filesystem::path _assets_dir;

  std::vector<Handler> _handlers = std::vector<Handler>();
  std::vector<Handler> _once_handlers = std::vector<Handler>();

public:
  App(std::unique_ptr<Control> control,
      RectSize screen_size,
      std::unique_ptr<DeferredShading> deferred_shading,
      std::unique_ptr<Shader> particle_shader,
      std::unique_ptr<Shader> intermediate_shader,
      WindowPtr window,
      ContextPtr gl_context,
      std::filesystem::path assets_dir);
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  App(App&&) = delete;
  App& operator=(App&&) = delete;
  ~App() = default;

  [[nodiscard]] Control& control() const;
  [[nodiscard]] DeferredShading& deferred_shading() const;
  [[nodiscard]] FrameBuffer& intermediate_fb() const;
  [[nodiscard]] Shader& particle_shader() const;
  [[nodiscard]] Shader& intermediate_shader() const;
  [[nodiscard]] SDL_Window& window() const;
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
};
