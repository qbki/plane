#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "app.h"
#include "game_loop.h"


void inner_game_loop(App& app) {
  SDL_Event event;
  auto& game_state = app.game_state;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        game_state->is_running(false);
        break;
      }
      case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_q) {
          game_state->is_running(false);
        }
        break;
      }
    }
    switch (event.window.event) {
      case SDL_WINDOWEVENT_RESIZED: {
        app.screen_size->width = event.window.data1;
        app.screen_size->height = event.window.data2;
        resize_window(event.window, *game_state->camera());
        app.deferred_shading->g_buffer()
          .update(app.screen_size->width, app.screen_size->height);
        break;
      }
    }
    app.control->update(event);
  }

  app.update(app, SDL_GetTicks64());

  SDL_GL_SwapWindow(app.window.get());
};

#ifdef __EMSCRIPTEN__
EM_BOOL emscripten_game_loop(double _, void* data) {
  inner_game_loop(*static_cast<App*>(data));
  return EM_TRUE;
}
void game_loop(App* app) {
  emscripten_request_animation_frame_loop(emscripten_game_loop, app);
}
#else
void game_loop(App* app) {
  while (app->game_state->is_running()) {
    inner_game_loop(*app);
  }
}
#endif
