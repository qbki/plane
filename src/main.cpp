#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include "camera.h"
#include "control.h"
#include "game_state/index.h"
#include "models/index.h"
#include "sdl_init.h"
#include "sun_light.h"
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

  auto factory = std::make_unique<ModelFactory>();
  auto scene = std::make_unique<Graphic>();
  auto game_state = std::make_unique<GameState>();

  auto light = std::make_unique<SunLight>(
    glm::vec3(1.0, 1.0, 1.0),
    glm::vec3(0.5, 0.5, 1.0)
  );

  game_state->camera(std::make_shared<Camera>(
    glm::vec3(0.0, -4.0, 15.0),
    static_cast<float>(screen_width) / screen_height
  ));

  {
    auto player = factory->make_player();
    player->position({0.0, -5.0, ANIMATED_OBJECTS_HEIGHT});
    game_state->player(player);
    scene->add_child(player);
  }

  {
    GameState::Entities enemies;
    for (int x = -10; x < 10; x++) {
      for (int y = 5; y < 30; y++) {
        auto model (factory->make_enemy());
        model->position({x, y, ANIMATED_OBJECTS_HEIGHT});
        enemies.push_back(model);
      }
    }
    game_state->add_enemies(enemies);
    scene->add_children(enemies);
  }

  {
    GameState::Entities bullets;
    {
      for (int x = 0; x < 100; x++) {
        auto model (factory->make_bullet());
        bullets.push_back(model);
      }
    }
    game_state->add_bullets(bullets);
    scene->add_children(bullets);
  }

  {
    GameState::Entities decorations;
    for (int y = -10; y < 10; y++) {
      for (int x = -15; x <= 15; x++) {
        auto model = rand() % 2 == 0
          ? factory->make_water_block()
          : factory->make_ground_block();
        model->position({x, y, 0.0});
        decorations.push_back(model);
      }
    }
    game_state->add_decoration(decorations);
    scene->add_children(decorations);
  }

  game_state->add_handler(make_cursor_handler(screen_width, screen_height));
  game_state->add_handler(move_player);
  game_state->add_handler(rotate_player);
  game_state->add_handler(shoot_by_player);
  game_state->add_handler(handle_bullets);
  game_state->add_handler(handle_enemies_hunting);
  game_state->add_handler(handle_enemy_rotation);
  game_state->add_handler(handle_enemy_sinking);

  auto is_running = true;
  auto last_time_point = SDL_GetTicks64();
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
          break;
        }
      }
      control.update(event);
    }

    auto now = SDL_GetTicks64();
    auto seconds_since_last_frame = (now - last_time_point) * 0.001;
    last_time_point = now;

    game_state->update(control, seconds_since_last_frame);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->draw(*game_state->camera(), *light, now * 0.001);

    SDL_GL_SwapWindow(window.get());
  }
}
