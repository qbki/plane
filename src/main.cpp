#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <memory>
#include <sstream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "camera.h"
#include "components.h"
#include "consts.h"
#include "control.h"
#include "game_state/index.h"
#include "lights/index.h"
#include "mesh.h"
#include "models/index.h"
#include "sdl_init.h"
#include "shading/index.h"
#include "systems/index.h"
#include "utils.h"


const int DEFAULT_SCREEN_WIDTH = 800;
const int DEFAULT_SCREEN_HEIGHT = 600;
const float ANIMATED_OBJECTS_HEIGHT = 2.0;

int main() {
  int screen_width = DEFAULT_SCREEN_WIDTH;
  int screen_height = DEFAULT_SCREEN_HEIGHT;
  auto window = init_window(screen_width, screen_height);
  auto context = init_context(window.get());
  Control control;

  auto game_state = std::make_unique<GameState>();
  auto common_material = std::make_unique<Material>(
    glm::vec3(0.01, 0.01, 0.01),
    glm::vec3(1.0, 1.0, 1.0),
    22
  );

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

  auto sun = std::make_unique<DirectionalLight>(
    glm::vec3(1.0, 1.0, 1.0),
    glm::vec3(-0.8, -0.8, -1.0)
  );

  std::vector<PointLight> lights {
    {{1, 0, 0}, {-5,  5, 8}},
    {{0, 1, 0}, {-5, -5, 8}},
    {{0, 0, 1}, { 5, -5, 8}},
    {{1, 0, 1}, { 0, -5, 8}},
  };

  for (auto& light : lights) {
    light.constant(1.0);
    light.linear(0.045);
    light.quadratic(0.0075);
  }

  game_state->camera(std::make_shared<Camera>(
    glm::vec3(0.0, -4.0, 15.0),
    static_cast<float>(screen_width) / screen_height
  ));

  game_state->factory().make_player(
    game_state->registry(),
    {0.0, -5.0, ANIMATED_OBJECTS_HEIGHT}
  );

  for (int x = -10; x < 10; x++) {
    for (int y = 0; y < 30; y++) {
      game_state->factory().make_enemy(
        game_state->registry(),
        {x, y, ANIMATED_OBJECTS_HEIGHT}
      );
    }
  }

  for (int y = -10; y < 10; y++) {
    for (int x = -15; x <= 15; x++) {
      auto& registry = game_state->registry();
      glm::vec3 position {x, y, 0.0};
      if (rand() % 2 == 0) {
        game_state->factory().make_water_block(registry, position);
      } else {
        game_state->factory().make_ground_block(registry, position);
      }
    }
  }

  auto deferred_shading = std::make_unique<DeferredShading>(
    std::move(geometry_pass_shader),
    std::move(light_pass_shader),
    Mesh::quad(0.99999),
    screen_width,
    screen_height
  );

  game_state->add_handler(make_cursor_handler_system(screen_width, screen_height));
  game_state->add_handler(enemy_hunting_system);
  game_state->add_handler(enemy_rotation_system);
  game_state->add_handler(enemy_sinking_system);
  game_state->add_handler(particle_handler_system);
  game_state->add_handler(player_moving_system);
  game_state->add_handler(player_rotation_system);
  game_state->add_handler(player_shooting_system);
  game_state->add_handler(velocity_gravity_system);
  game_state->add_handler(velocity_system);
  game_state->add_handler(projectile_handler_system);
  game_state->add_handler(camera_move_system);
  game_state->add_handler(make_render_system(
    *deferred_shading,
    *particle_shader,
    *common_material,
    *sun,
    lights,
    screen_width,
    screen_height
  ));

  auto is_running = true;
  SDL_Event event;

  while (is_running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          is_running = false;
          break;
        }
        case SDL_KEYDOWN: {
          if (event.key.keysym.sym == SDLK_q) {
            is_running = false;
          }
          break;
        }
      }
      switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          screen_width = event.window.data1;
          screen_height = event.window.data2;
          resize_window(event.window, *game_state->camera());
          deferred_shading->g_buffer().update(screen_width, screen_height);
          break;
        }
      }
      control.update(event);
    }

    game_state->update(control, SDL_GetTicks64());

    SDL_GL_SwapWindow(window.get());
  }
}
