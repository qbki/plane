#include "logger/logger.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <memory>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "app.h"
#include "camera.h"
#include "components.h"
#include "consts.h"
#include "control.h"
#include "game_loop.h"
#include "game_state/index.h"
#include "mesh.h"
#include "models/index.h"
#include "sdl_init.h"
#include "shading/index.h"
#include "systems/index.h"
#include "service.h"
#include "logger/index.h"


const int DEFAULT_SCREEN_WIDTH = 800;
const int DEFAULT_SCREEN_HEIGHT = 600;
const float ANIMATED_OBJECTS_HEIGHT = 2.0;


int main() {
  Service<AbstractLogger>::install(std::make_unique<Logger>());

  auto window = init_window(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  auto context = init_context(window.get());
  auto control = std::make_unique<Control>();

  auto game_state = std::make_unique<GameState>();
  auto& factory = game_state->factory();
  auto& registry = game_state->registry();

  auto geometry_pass_shader = std::make_unique<Shader>();
  auto geometry_pass_vertex_shader = load_text("./shaders/main_v.glsl");
  auto geometry_pass_fragment_shader = load_text("./shaders/deferred_geometry_pass_f.glsl");
  geometry_pass_shader->compile(geometry_pass_vertex_shader, geometry_pass_fragment_shader);

  auto light_pass_shader = std::make_unique<Shader>();
  auto light_pass_vertex_shader = load_text("./shaders/deferred_light_pass_v.glsl");
  auto light_pass_fragment_shader = load_text("./shaders/deferred_light_pass_f.glsl");
  light_pass_shader->compile(light_pass_vertex_shader, light_pass_fragment_shader);

  auto particle_shader = std::make_unique<Shader>();
  auto particle_vertex_shader = load_text("./shaders/main_v.glsl");
  auto particle_fragment_shader = load_text("./shaders/particle_f.glsl");
  particle_shader->compile(particle_vertex_shader, particle_fragment_shader);

  factory.make_directional_light(registry, {-0.8, -0.8, -1.0}, {1.0, 1.0, 1.0});
  factory.make_point_light(registry, {-5,  5,  5}, {1, 0, 0});
  factory.make_point_light(registry, {-5, -5,  5}, {0, 1, 0});
  factory.make_point_light(registry, { 5, -5,  5}, {0, 0, 1});
  factory.make_point_light(registry, { 0, -5,  5}, {1, 0, 1});

  game_state->camera(std::make_shared<Camera>(
    glm::vec3(0.0, -4.0, 15.0),
    static_cast<float>(DEFAULT_SCREEN_WIDTH) / DEFAULT_SCREEN_HEIGHT
  ));

  auto player_id = factory.make_player(
    registry,
    {0.0, -5.0, ANIMATED_OBJECTS_HEIGHT}
  );
  game_state->player_id(player_id);

  for (int x = -10; x < 10; x++) {
    for (int y = 0; y < 30; y++) {
      factory.make_enemy(
        registry,
        {x, y, ANIMATED_OBJECTS_HEIGHT}
      );
    }
  }

  for (int y = -10; y < 10; y++) {
    for (int x = -15; x <= 15; x++) {
      glm::vec3 position {x, y, 0.0};
      if (rand() % 2 == 0) {
        factory.make_water_block(registry, position);
      } else {
        factory.make_ground_block(registry, position);
      }
    }
  }

  auto deferred_shading = std::make_unique<DeferredShading>(
    std::move(geometry_pass_shader),
    std::move(light_pass_shader),
    Mesh::quad(0.99999),
    DEFAULT_SCREEN_WIDTH,
    DEFAULT_SCREEN_HEIGHT
  );

  auto app = AppBuilder()
    .game_state(std::move(game_state))
    .control(std::move(control))
    .screen_size(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT)
    .deferred_shading(std::move(deferred_shading))
    .particle_shader(std::move(particle_shader))
    .window(std::move(window))
    .context(std::move(context))
    .build();

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
  app->add_handler(render_system);

  game_loop(app);

#ifdef __EMSCRIPTEN__
#else
  delete app;
#endif
}
