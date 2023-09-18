#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/ext.hpp>

#include "./cache.h"
#include "./camera.h"
#include "./control.h"
#include "./graphic.h"
#include "./model.h"
#include "./sdl_init.h"
#include "./sun_light.h"
#include "./utils.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main() {
  int screen_width = SCREEN_WIDTH;
  int screen_height = SCREEN_HEIGHT;
  auto window = init_window(screen_width, screen_height);
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


  std::vector<std::shared_ptr<Graphic>> enemies;
  {
    for (int x = -10; x < 10; x++) {
      for (int y = -10; y < 10; y++) {
        auto model(cache->load(
          "./models/plane.glb",
          "./shaders/main_v.glsl",
          "./shaders/main_f.glsl"
        ));
        model->set_position({x, y, 2.0});
        root->add_child(model);
        enemies.push_back(model);
      }
    }
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
          if (event.key.keysym.sym == SDLK_q) {
            is_running = false;
          }
        }
      }
      switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          screen_width = event.window.data1;
          screen_height = event.window.data2;
          resize_window(event.window, *camera);
          break;
        }
      }
      control.update(event);
    }

    auto now = SDL_GetTicks64();
    auto seconds_since_last_frame = (now - last_time_point) * 0.001;
    last_time_point = now;

    int mouse_x = 0;
    int mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);

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

    glm::vec4 viewport{0, 0, screen_width, screen_height};
    auto projection_point = glm::unProject(
      {mouse_x, screen_height - mouse_y, 1.0},
      camera->get_view(),
      camera->get_projection(),
      viewport
    );

    auto ray = glm::normalize(projection_point - camera->get_position());

    float intersection_distance = 0;
    auto has_intersection = glm::intersectRayPlane(
      camera->get_position(),
      ray,
      {0.0, 0.0, 2.0},
      {0.0, 0.0, 1.0},
      intersection_distance
    );
    if (has_intersection) {
      auto point = (camera->get_position() + ray * intersection_distance) - player->get_position();
      auto angle = glm::atan2(point.y, point.x) - glm::half_pi<float>();
      player->set_rotation_z(angle);
    }

    for (auto& enemy : enemies) {
      auto pos_a {enemy->get_position()};
      glm::vec3 sum {0, 0, 0};
      for (auto& near_enemy : enemies) {
        if (enemy == near_enemy) {
          continue;
        }
        auto pos_b {near_enemy->get_position()};
        if (glm::distance(pos_a, pos_b) < 0.7) {
          sum = glm::normalize(sum + glm::normalize(pos_a - pos_b));
        }
      }
      sum = glm::normalize(sum + glm::normalize(player->get_position() - pos_a) * 0.05);
      enemy->move_in(sum, 1.0 * seconds_since_last_frame);

      auto direction_vector = player->get_position() - enemy->get_position();
      auto rotation = glm::atan2(direction_vector.y, direction_vector.x) - glm::half_pi<float>();
      enemy->set_rotation_z(rotation);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    root->draw(*camera, *light, now * 0.001);

    SDL_GL_SwapWindow(window.get());
  }
}
