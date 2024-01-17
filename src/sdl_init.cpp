#include <GL/glew.h>
#include <SDL.h>
#include <SDL_video.h>
#include <format>
#include <stdexcept>
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "src/utils/gl.h"

#include "sdl_init.h"
#include "services.h"

void
throw_sdl_error(std::string message)
{
  throw new std::runtime_error(std::format("{}: {}", message, SDL_GetError()));
}

WindowPtr
init_window(int screen_width, int screen_height)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw_sdl_error("Unable to init SDL");
  }
  logger().info("SDL has been initialized.");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  auto window = SDL_CreateWindow("Plane",
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 screen_width,
                                 screen_height,
                                 SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL |
                                   SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
  if (window == nullptr) {
    throw_sdl_error("Unable to create window");
  }
  logger().info("Window has been created.");

  return { window, [](auto w) {
            SDL_DestroyWindow(w);
            SDL_Quit();
          } };
}

ContextPtr
init_context(SDL_Window* window)
{
  auto ctx = SDL_GL_CreateContext(window);
  if (ctx == nullptr) {
    throw_sdl_error("Unable to create GL Context");
  }
  logger().info("Context has been created.");

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    auto err_glew = glubyte_to_string(glewGetErrorString(err));
    throw new std::runtime_error(
      std::format("Unable to initialize GLEW: {}", err_glew));
  }
  logger().info("GLEW has been inited.");

  print_opengl_info();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return { ctx, [](auto c) { SDL_GL_DeleteContext(c); } };
}
