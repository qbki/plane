#include <GL/glew.h>
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_video.h>
#include <format>
#include <stdexcept>
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "src/consts.h"
#include "src/utils/gl.h"

#include "sdl_init.h"
#include "services.h"

void
throw_sdl_error(std::string message)
{
  throw std::runtime_error(std::format("{}: {}", message, SDL_GetError()));
}

void GLAPIENTRY
MessageCallback(GLenum /*source*/,
                GLenum /*type*/,
                GLuint /*id*/,
                GLenum /*severity*/,
                GLsizei /*length*/,
                const GLchar* message,
                const void* /*userParam*/)
{
  Services::logger().error(std::format("OpenGL: {}", message));
}

WindowPtr
init_window(int screen_width, int screen_height)
{
  // SDL
  int error = 0;
  error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (error < 0) {
    throw_sdl_error("Unable to init SDL");
  }
  Services::logger().info("SDL has been initialized.");

  // Audio
  error = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                        MIX_DEFAULT_FORMAT,
                        MIX_DEFAULT_CHANNELS,
                        DEFAULT_AUDIO_CHUNKSIZE);
  if (error < 0) {
    throw_sdl_error("Unable to init SDL_mixer");
  }
  Mix_AllocateChannels(DEFAULT_MAX_CHANNELS);
  Services::logger().info("SDL_mixer has been initialized.");

  // Font
  error = TTF_Init();
  if (error < 0) {
    throw_sdl_error("Unable to init SDL_ttf");
  }
  Services::logger().info("SDL_ttf has been initialized.");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#ifdef __EMSCRIPTEN__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif
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
  Services::logger().info("Window has been created.");

  return { window, [](auto w) {
            SDL_DestroyWindow(w);
            TTF_Quit();
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
  Services::logger().info("Context has been created.");

  auto swap_interwal_error = SDL_GL_SetSwapInterval(1);
  if (swap_interwal_error < 0) {
    throw_sdl_error("Unable to enable VSync");
  }

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    auto err_glew = glubyte_to_string(glewGetErrorString(err));
    throw new std::runtime_error(
      std::format("Unable to initialize GLEW: {}", err_glew));
  }
  Services::logger().info("GLEW has been inited.");

  print_opengl_info();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef __EMSCRIPTEN__
  // glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, nullptr);
#endif

  return { ctx, [](auto c) { SDL_GL_DeleteContext(c); } };
}
