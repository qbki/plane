#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <cstdlib>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/ext.hpp>

#include "./sdl_init.h"
#include "./model.h"
#include "./cache.h"
#include "./camera.h"
#include "./graphic.h"
#include "./sun_light.h"
#include "./utils.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Control {
  bool up;
  bool down;
  bool left;
  bool right;

  Control() :
    up(false),
    down(false),
    left(false),
    right(false) {}
};

int main() {
  auto window = init_window(SCREEN_WIDTH, SCREEN_HEIGHT);
  auto context = init_context(window.get());
  Control control;

  // SDL_SetRelativeMouseMode(SDL_TRUE);

  std::unique_ptr<Graphic> root(new Graphic());

  std::unique_ptr<SunLight> light(new SunLight(
    {1.0, 1.0, 1.0},
    {0.5, 0.5, 1.0}
  ));

  std::unique_ptr<Cache> cache(new Cache());

  std::unique_ptr<Camera> camera(new Camera(
    {0.0, -4.0, 15.0},
    static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT
  ));

  std::shared_ptr<Graphic> player;
  {
    auto model(cache->load(
      "./models/plane.glb",
      "./shaders/main_v.glsl",
      "./shaders/main_f.glsl"
    ));
    model->set_position({0.0, -5.0, 2.0});
    root->add_child(model);
    player = model;
  }

  auto generate_line = [&](float y) {
    for (int x = -15; x <= 15; x++) {
      if (rand() % 2 == 0) {
        auto model(cache->load(
          "./models/water-surface.glb",
          "./shaders/water_v.glsl",
          "./shaders/main_f.glsl"
        ));
        model->set_position({static_cast<float>(x), static_cast<float>(y), 0.0});
        root->add_child(model);
      } else {
        auto model(cache->load(
          "./models/center-block.glb",
          "./shaders/main_v.glsl",
          "./shaders/main_f.glsl"
        ));
        model->set_position({static_cast<float>(x), static_cast<float>(y), 0.0});
        root->add_child(model);
      }
    }
  };

  for (int y = -10; y < 10; y++) {
    generate_line(y);
  }

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
          if (event.key.keysym.sym == SDLK_a) {
            control.left = true;
          }
          if (event.key.keysym.sym == SDLK_d) {
            control.right = true;
          }
          if (event.key.keysym.sym == SDLK_w) {
            control.up = true;
          }
          if (event.key.keysym.sym == SDLK_s) {
            control.down = true;
          }
          break;
        }
        case SDL_KEYUP: {
          if (event.key.keysym.sym == SDLK_a) {
            control.left = false;
          }
          if (event.key.keysym.sym == SDLK_d) {
            control.right = false;
          }
          if (event.key.keysym.sym == SDLK_w) {
            control.up = false;
          }
          if (event.key.keysym.sym == SDLK_s) {
            control.down = false;
          }
          break;
        }
      }
      switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          resize_window(event.window, *camera);
          break;
        }
      }
    }

    auto now = SDL_GetTicks64();
    auto seconds_since_last_frame = (now - last_time_point) * 0.001;
    last_time_point = now;

    glm::vec3 move_normal {0, 0, 0};
    if (control.left) {
      move_normal.x -= 1;
    }
    if (control.right) {
      move_normal.x += 1;
    }
    if (control.up) {
      move_normal.y += 1;
    }
    if (control.down) {
      move_normal.y -= 1;
    }
    if (!is_approx_equal(glm::length2(move_normal), 0.0f)) {
      move_normal = glm::normalize(move_normal);
    }
    player->move_in(move_normal, 5.0 * seconds_since_last_frame);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    root->draw(*camera, *light, now * 0.001);

    SDL_GL_SwapWindow(window.get());
  }
}
