module;
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_error.h>
#include <SDL_mixer.h>
#include <SDL_video.h>
#include <cstddef>
#include <format>
#include <memory>
#include <string>
#include <thorvg.h>
#include <thread>
#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

#include "src/utils/gl.h"
#include "src/utils/tvg.h"

export module pln.sdl;

import pln.consts;
import pln.gui.ui_canvas;
import pln.services.logger;
import pln.utils.crash;
import pln.utils.platform;

namespace pln::sdl {

export using WindowPtr = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>;
export using ContextPtr = std::unique_ptr<void, void (*)(SDL_GLContext)>;
export using UiCanvasPtr = std::unique_ptr<pln::gui::UiCanvas>;

void
crash_with_sdl_error(std::string message)
{
  pln::utils::crash(std::format("{}: {}", message, SDL_GetError()));
}

void GLAPIENTRY
messageCallback(GLenum /*source*/,
                GLenum /*type*/,
                GLuint /*id*/,
                GLenum /*severity*/,
                GLsizei /*length*/,
                const GLchar* message,
                const void* /*userParam*/)
{
  pln::services::logger().error(std::format("OpenGL: {}", message));
}

export
WindowPtr
init_window(int screen_width, int screen_height)
{
  // SDL
  int error = 0;
  error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (error < 0) {
    crash_with_sdl_error("Unable to init SDL");
  }
  pln::services::logger().info("SDL has been initialized.");

  // Audio
  error = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                        MIX_DEFAULT_FORMAT,
                        MIX_DEFAULT_CHANNELS,
                        pln::consts::DEFAULT_AUDIO_CHUNKSIZE);
  if (error < 0) {
    crash_with_sdl_error("Unable to init SDL_mixer");
  }
  Mix_AllocateChannels(pln::consts::DEFAULT_MAX_CHANNELS);
  pln::services::logger().info("SDL_mixer has been initialized.");

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
                                 SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
                                   | SDL_WINDOW_RESIZABLE
                                   | SDL_WINDOW_MAXIMIZED);
  if (window == nullptr) {
    crash_with_sdl_error("Unable to create window");
  }
  pln::services::logger().info("Window has been created.");

  unsigned int threads = pln::utils::platform::IS_WEB
                         ? 0
                         : std::thread::hardware_concurrency();
  auto tvg_result = tvg::Initializer::init(tvg::CanvasEngine::Sw, threads);
  vg_verify_or_crash(__func__, tvg_result);
  pln::services::logger().info("ThorVG has been initialized.");

  return { window, [](auto w) {
            tvg::Initializer::term(tvg::CanvasEngine::Sw);
            SDL_DestroyWindow(w);
            SDL_Quit();
          } };
}

export
ContextPtr
init_context(SDL_Window* window)
{
  auto ctx = SDL_GL_CreateContext(window);
  if (ctx == nullptr) {
    crash_with_sdl_error("Unable to create GL Context");
  }
  pln::services::logger().info("Context has been created.");

  auto swap_interwal_error = SDL_GL_SetSwapInterval(1);
  if (swap_interwal_error < 0) {
    crash_with_sdl_error("Unable to enable VSync");
  }

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    auto err_glew = glubyte_to_string(glewGetErrorString(err));
    pln::utils::crash(std::format("Unable to initialize GLEW: {}", err_glew));
  }
  pln::services::logger().info("GLEW has been inited.");

  print_opengl_info();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef __EMSCRIPTEN__
  // glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(messageCallback, nullptr);
#endif

  return { ctx, [](auto c) { SDL_GL_DeleteContext(c); } };
}

export
UiCanvasPtr
init_vg_canvas(std::size_t width, std::size_t height)
{
  return std::make_unique<pln::gui::UiCanvas>(width, height);
}

}
