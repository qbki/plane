#pragma once
#include <SDL_video.h> // IWYU pragma: export
#include <cstddef>
#include <memory>

#include "src/gui/ui_canvas.h"

using WindowPtr = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>;
using ContextPtr = std::unique_ptr<void, void (*)(SDL_GLContext)>;
using UiCanvasPtr = std::unique_ptr<UiCanvas>;

WindowPtr
init_window(int screen_width, int screen_height);

ContextPtr
init_context(SDL_Window* window);

UiCanvasPtr
init_vg_canvas(std::size_t width, std::size_t height);
