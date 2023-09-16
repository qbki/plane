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

glm::vec3 get_top_line_coordinates(Camera& camera) {
  glm::vec4 viewport{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

  auto top_ray = glm::normalize(glm::unProject(
    {400.0, 600.0, 1.0},
    camera.get_view(),
    camera.get_projection(),
    viewport
  ));

  float intersection_distance = 0;
  auto has_intersection = glm::intersectRayPlane(
    camera.get_position(),
    top_ray,
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    intersection_distance
  );

  return camera.get_position() + top_ray * intersection_distance;
}


int main() {
  auto window = init_window(SCREEN_WIDTH, SCREEN_HEIGHT);
  auto context = init_context(window.get());

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

  auto top_line = get_top_line_coordinates(*camera);

  for (int y = -10; y < top_line.y; y++) {
    generate_line(y);
  }

  bool is_running = true;
  SDL_Event event;

  while (is_running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          is_running = false;
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

    player->set_position(player->get_position() + glm::vec3(0.0, 0.01, 0.0));
    camera->set_position(camera->get_position() + glm::vec3(0.0, 0.01, 0.0));

    auto new_top_line = get_top_line_coordinates(*camera);
    if (std::floor(new_top_line.y) > std::floor(top_line.y)) {
      generate_line(std::floor(new_top_line.y));
      top_line = new_top_line;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto seconds_since_last_frame = SDL_GetTicks64() * 0.001f;
    root->draw(*camera, *light, seconds_since_last_frame);

    SDL_GL_SwapWindow(window.get());
  }
}
