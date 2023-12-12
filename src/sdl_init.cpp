#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>
#include <SDL_video.h>
#include <iostream>
#include <stdexcept>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "sdl_init.h"
#include "utils.h"


void throw_sdl_error(std::string message) {
  auto err_text = message + ": " + SDL_GetError();
  print_error(err_text);
  throw new std::runtime_error(err_text);
}


WindowPtr init_window(uint screen_width, uint screen_height) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw_sdl_error("Unable to init SDL");
  }
  std::cout << "SDL has been initialized." << std::endl;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  auto window = SDL_CreateWindow(
    "Plane", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width,
    screen_height,
    SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
  if (window == nullptr) {
    throw_sdl_error("Unable to create window");
  }
  std::cout << "Window has been created." << std::endl;

  return {
    window,
    [](auto w) {
      SDL_DestroyWindow(w);
      SDL_Quit();
    }
  };
}


ContextPtr init_context(SDL_Window* window) {
  auto ctx = SDL_GL_CreateContext(window);
  if (ctx == nullptr) {
    throw_sdl_error("Unable to create GL Context");
  }
  std::cout << "Context has been created." << std::endl;

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    auto err_glew = reinterpret_cast<const char*>(glewGetErrorString(err));
    auto err_text = std::string("Unable to initialize GLEW: ") + err_glew;
    throw new std::runtime_error(err_text);
  }
  std::cout << "GLEW has been inited." << std::endl;

  print_opengl_info();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return {
    ctx,
    [](auto c) { SDL_GL_DeleteContext(c); }
  };
}
