module;
#include <GL/glew.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

export module pln.game_loop;

import pln.app.app;
import pln.scene.iscene;
import pln.services.app;

namespace pln {

void
resize_cameras(const std::vector<std::unique_ptr<pln::scene::IScene>>& scenes,
               int width,
               int height)
{
  for (const auto& scene : scenes) {
    scene->state().camera().screen_size(width, height);
  }
}

void
resize_screens(pln::app::App& app, int width, int height)
{
  app.screen_size({ width, height });
  resize_cameras(app.scenes(), width, height);
  app.deferred_shading().g_buffer().update(width, height);
  app.intermediate_fb().update(width, height);
  glViewport(0, 0, width, height);
}

#ifdef __EMSCRIPTEN__
void
wasm_resize_window(pln::app::App& app, int width, int height)
{
  auto& window = app.window();
  SDL_SetWindowSize(&window, width, height);
  resize_screens(app, width, height);
}

EM_BOOL
wasm_resize_window_cb(int /* eventType */,
                      const EmscriptenUiEvent* uiEvent,
                      void* data)
{
  int width = uiEvent->windowInnerWidth;
  int height = uiEvent->windowInnerHeight;
  auto application = static_cast<pln::app::App*>(data);
  wasm_resize_window(*application, width, height);
  return EMSCRIPTEN_RESULT_SUCCESS;
}

EM_JS(int, get_js_window_width, (), { return window.innerWidth; });

EM_JS(int, get_js_window_height, (), { return window.innerHeight; });
#endif

void
inner_game_loop(pln::app::App& app)
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_EVENT_QUIT: {
        app.is_running(false);
        break;
      }
      case SDL_EVENT_KEY_DOWN: {
        if (event.key.key == SDLK_Q) {
          app.is_running(false);
        }
        break;
      }
      case SDL_EVENT_WINDOW_RESIZED: {
        auto width = event.window.data1;
        auto height = event.window.data2;
        resize_screens(app, width, height);
        break;
      }
    }
    app.control().update(event);
  }

  app.update(SDL_GetTicks());

  SDL_GL_SwapWindow(&app.window());
};

#ifdef __EMSCRIPTEN__

void
emscripten_game_loop(void* data)
{
  inner_game_loop(*static_cast<pln::app::App*>(data));
}

export
void
game_loop()
{
  auto& app = pln::services::app();
  wasm_resize_window(app, get_js_window_width(), get_js_window_height());
  emscripten_set_resize_callback(
    EMSCRIPTEN_EVENT_TARGET_WINDOW, &app, EM_FALSE, wasm_resize_window_cb);
  // The simulate_infinite_loop argument should be false. It is because I have
  // nothing on the stack what should be kept. And it helps me avoid
  // unnecessary exception that is thrown intentionally by emscripten. More
  // details in the documentation below.
  // @doc
  // https://emscripten.org/docs/api_reference/emscripten.h.html?highlight=set_main_loop#c.emscripten_set_main_loop
  emscripten_set_main_loop_arg(emscripten_game_loop, &app, 0, false);
}

#else

export
void
game_loop()
{
  auto& app = pln::services::app();
  while (app.is_running()) {
    inner_game_loop(app);
  }
}

#endif

}
