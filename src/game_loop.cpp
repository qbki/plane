#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "app.h"
#include "game_loop.h"

void
resize_window(const SDL_WindowEvent& window_event, Camera& camera)
{
  auto width = window_event.data1;
  auto height = window_event.data2;
  camera.aspect_ratio(static_cast<float>(width) / static_cast<float>(height));
  glViewport(0, 0, width, height);
}

void
inner_game_loop(App& app)
{
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
        app.deferred_shading->g_buffer().update(app.screen_size->width,
                                                app.screen_size->height);
        break;
      }
    }
    app.control->update(event);
  }

  app.update(SDL_GetTicks64());

  SDL_GL_SwapWindow(app.window.get());
};

#ifdef __EMSCRIPTEN__
void
emscripten_game_loop(void* data)
{
  inner_game_loop(*static_cast<App*>(data));
}
void
game_loop(App* app)
{
  // The simulate_infinite_loop argument should be false. It is because I have
  // nothing on the stack what should be kept. And it helps me avoid
  // unnecessary exception that is thrown intentionally by emscripten. More
  // details in the documentation below.
  // @doc
  // https://emscripten.org/docs/api_reference/emscripten.h.html?highlight=set_main_loop#c.emscripten_set_main_loop
  emscripten_set_main_loop_arg(emscripten_game_loop, app, 0, false);
}
#else
void
game_loop(App* app)
{
  while (app->game_state->is_running()) {
    inner_game_loop(*app);
  }
}
#endif
