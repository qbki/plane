#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include "./model.h"
#include "./cache.h"
#include "./camera.h"
#include "./graphic.h"
#include "./sun_light.h"
#include "./utils.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    auto err_text = std::string("Unable to init SDL: ") + SDL_GetError();
    throw new std::runtime_error(err_text);
  }
  std::cout << "SDL has been initialized." << std::endl;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  auto window = SDL_CreateWindow(
    "Plane",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH, SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (window == nullptr) {
    auto err_sdl = std::unique_ptr<const char>(SDL_GetError());
    auto err_text = std::string("Unable to create window: ") + *err_sdl;
    throw new std::runtime_error(err_text);
  }
  std::cout << "Window has been created." << std::endl;

  auto ctx = SDL_GL_CreateContext(window);
  if (ctx == nullptr) {
    auto err_sdl = std::unique_ptr<const char>(SDL_GetError());
    auto err_text = std::string("Unable to create GL Context: ") + *err_sdl;
    throw new std::runtime_error(err_text);
  }
  std::cout << "Context has been created." << std::endl;

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    auto err_glew = std::unique_ptr<const char>(reinterpret_cast<const char*>(glewGetErrorString(err)));
    auto err_text = std::string() + *err_glew;
    throw new std::runtime_error(err_text);
  }
  std::cout << "GLEW has been inited." << std::endl;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // SDL_SetRelativeMouseMode(SDL_TRUE);

  std::unique_ptr<Graphic> root(new Graphic());

  std::unique_ptr<Camera> camera(new Camera(
    {0.0, 0.0, 10.0},
    static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT
  ));

  std::unique_ptr<SunLight> light(new SunLight(
    {1.0, 1.0, 1.0},
    {0.2, 0.2, 1.0}
  ));

  std::unique_ptr<Cache> cache(new Cache());

  {
    auto model(cache->load(
      "./models/plane.glb",
      "./shaders/main_v.glsl",
      "./shaders/main_f.glsl"
    ));
    model->set_position({-0.5, 0.0, 0.0});
    root->add_child(model);
  }

  {
    auto model(cache->load(
      "./models/plane.glb",
      "./shaders/main_v.glsl",
      "./shaders/main_f.glsl"
    ));
    model->set_position({0.5, 0.0, 0.0});
    root->add_child(model);
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    root->draw(*camera, *light);
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(window);
}
