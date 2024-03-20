#pragma once
#include <SDL.h> // IWYU pragma: export
#include <memory>

using WindowPtr = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>;
using ContextPtr = std::unique_ptr<void, void (*)(SDL_GLContext)>;

WindowPtr
init_window(int screen_width, int screen_height);

ContextPtr
init_context(SDL_Window* window);
