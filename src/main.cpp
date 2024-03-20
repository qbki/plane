#define GLM_ENABLE_EXPERIMENTAL
#include <gsl/pointers>
#include <utility>

#include "consts.h"
#include "game_loop.h"
#include "mesh.h"
#include "sdl_init.h"
#include "src/loader/level_loader.h"
#include "src/logger/abstract_logger.h"
#include "src/logger/logger.h"
#include "src/service.h"
#include "src/systems/camera.h"
#include "src/systems/cursor.h"
#include "src/systems/enemy.h"
#include "src/systems/particles.h"
#include "src/systems/player.h"
#include "src/systems/projectiles.h"
#include "src/systems/render.h"
#include "src/systems/tutorial_buttons.h"
#include "src/systems/velocity.h"
#include "utils/file_loaders.h"

int
main()
{
  Service<AbstractLogger>::install(std::make_unique<Logger>());

  auto window = init_window(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  auto context = init_context(window.get());
  auto control = std::make_unique<Control>();

  auto game_state = std::make_unique<GameState>();

  auto geometry_pass_shader = std::make_unique<Shader>();
  auto geometry_pass_vertex_shader = load_text("./shaders/main_v.glsl");
  auto geometry_pass_fragment_shader =
    load_text("./shaders/deferred_geometry_pass_f.glsl");
  geometry_pass_shader->compile(geometry_pass_vertex_shader,
                                geometry_pass_fragment_shader);

  auto light_pass_shader = std::make_unique<Shader>();
  auto light_pass_vertex_shader =
    load_text("./shaders/deferred_light_pass_v.glsl");
  auto light_pass_fragment_shader =
    load_text("./shaders/deferred_light_pass_f.glsl");
  light_pass_shader->compile(light_pass_vertex_shader,
                             light_pass_fragment_shader);

  auto particle_shader = std::make_unique<Shader>();
  auto particle_vertex_shader = load_text("./shaders/main_v.glsl");
  auto particle_fragment_shader = load_text("./shaders/particle_f.glsl");
  particle_shader->compile(particle_vertex_shader, particle_fragment_shader);

  game_state->camera(std::make_shared<Camera>(
    static_cast<float>(DEFAULT_SCREEN_WIDTH) / DEFAULT_SCREEN_HEIGHT));

  auto deferred_shading =
    std::make_unique<DeferredShading>(std::move(geometry_pass_shader),
                                      std::move(light_pass_shader),
                                      Mesh::quad(FARTHEST_NDS_Z_COORD),
                                      DEFAULT_SCREEN_WIDTH,
                                      DEFAULT_SCREEN_HEIGHT);

  AppBuilder app_builder = AppBuilder();
  app_builder.screen_size(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  app_builder.context(std::move(context));
  app_builder.control(std::move(control));
  app_builder.deferred_shading(std::move(deferred_shading));
  app_builder.game_state(std::move(game_state));
  app_builder.particle_shader(std::move(particle_shader));
  app_builder.window(std::move(window));
  // The app variable must overlive the main function because of
  // async nature of requestanimationframe in browser api (emscripten)
  gsl::owner<App*> app{ app_builder.build() };

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
  app->add_handler(render_system);

  load_level(
    "./assets/levels/entities.json", "./assets/levels/level_1.json", *app);

  game_loop(app);

#ifndef __EMSCRIPTEN__
  delete app;
#endif
}
