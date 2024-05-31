#define GLM_ENABLE_EXPERIMENTAL
#include <SDL_video.h>
#include <filesystem>
#include <gsl/pointers>
#include <utility>

#include "app/app_builder.h"
#include "cache/cache.h"
#include "cameras/gui_camera.h"
#include "cameras/perspective_camera.h"
#include "consts.h"
#include "events/event.h"
#include "events/event_emitter.h"
#include "game_loop.h"
#include "gui/game_screen_factory.h"
#include "gui/theme.h"
#include "loader/level_loader.h"
#include "loader/theme_loader.h"
#include "logger/abstract_logger.h"
#include "logger/logger.h"
#include "mesh.h"
#include "sdl_init.h"
#include "service.h"
#include "services.h"
#include "systems/calculate_world_bbox.h"
#include "systems/camera.h"
#include "systems/cursor.h"
#include "systems/enemy.h"
#include "systems/particles.h"
#include "systems/player.h"
#include "systems/projectiles.h"
#include "systems/render.h"
#include "systems/tutorial_buttons.h"
#include "systems/update_gui.h"
#include "systems/velocity.h"
#include "utils/file_loaders.h"

int
main()
{
  Service<AbstractLogger>::install(std::make_unique<Logger>());
  Service<Cache>::install(std::make_unique<Cache>());
  Service<Events::EventEmitter<Events::ShootEvent>>::install(
    std::make_unique<Events::EventEmitter<Events::ShootEvent>>());

  auto window = init_window(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  auto context = init_context(window.get());
  auto control = std::make_unique<Control>();

  auto game_state = std::make_unique<GameState>();

  auto geometry_pass_shader = std::make_unique<Shader>();
  auto geometry_pass_vertex_shader = load_text(SHADERS_DIR / "main_v.glsl");
  auto geometry_pass_fragment_shader =
    load_text(SHADERS_DIR / "deferred_geometry_pass_f.glsl");
  geometry_pass_shader->compile(geometry_pass_vertex_shader,
                                geometry_pass_fragment_shader);

  auto light_pass_shader = std::make_unique<Shader>();
  auto light_pass_vertex_shader =
    load_text(SHADERS_DIR / "deferred_light_pass_v.glsl");
  auto light_pass_fragment_shader =
    load_text(SHADERS_DIR / "deferred_light_pass_f.glsl");
  light_pass_shader->compile(light_pass_vertex_shader,
                             light_pass_fragment_shader);

  auto particle_shader = std::make_unique<Shader>();
  auto particle_vertex_shader = load_text(SHADERS_DIR / "main_v.glsl");
  auto particle_fragment_shader = load_text(SHADERS_DIR / "particle_f.glsl");
  particle_shader->compile(particle_vertex_shader, particle_fragment_shader);

  std::unique_ptr<Camera> camera = std::make_unique<PerspectiveCamera>(
    DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  game_state->camera(camera);

  std::unique_ptr<Camera> gui_camera =
    std::make_unique<GUICamera>(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  game_state->gui_camera(gui_camera);

  auto deferred_shading = std::make_unique<DeferredShading>(
    std::move(geometry_pass_shader),
    std::move(light_pass_shader),
    Mesh::quad(1.0, 1.0, FARTHEST_NDS_Z_COORD),
    DEFAULT_SCREEN_WIDTH,
    DEFAULT_SCREEN_HEIGHT);

  auto theme = load_theme(ASSETS_DIR / "levels/theme.json");

  AppBuilder app_builder = AppBuilder();
  app_builder.screen_size(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  app_builder.context(std::move(context));
  app_builder.control(std::move(control));
  app_builder.deferred_shading(std::move(deferred_shading));
  app_builder.game_state(std::move(game_state));
  app_builder.particle_shader(std::move(particle_shader));
  app_builder.window(std::move(window));
  app_builder.theme(std::move(theme));
  // The app variable must overlive the main function because of
  // async nature of requestanimationframe in browser api (emscripten)
  gsl::owner<App*> app{ app_builder.build() };

  app->add_once_handler(calculate_world_bbox);
  app->add_once_handler(game_screen_factory);
  app->add_handler(cursor_handler_system);
  app->add_handler(enemy_hunting_system);
  app->add_handler(enemy_rotation_system);
  app->add_handler(enemy_sinking_system);
  app->add_handler(particle_handler_system);
  app->add_handler(player_moving_system);
  app->add_handler(player_rotation_system);
  app->add_handler(player_shooting_system);
  app->add_handler(velocity_gravity_system);
  app->add_handler(velocity_system);
  app->add_handler(projectile_handler_system);
  app->add_handler(camera_move_system);
  app->add_handler(tutorial_buttons_system);
  app->add_handler(update_gui);
  app->add_handler(render_system);

  std::function<void(const Events::ShootEvent&)> cb = [](const auto& event) {
    cache().get_sound(event.sound_path)->play();
  };
  events<Events::ShootEvent>().add(cb);

  load_level(ASSETS_DIR / "levels/entities.json",
             ASSETS_DIR / "levels/level_1.json",
             *app);

  game_loop(app);

#ifndef __EMSCRIPTEN__
  delete app;
#endif
}
