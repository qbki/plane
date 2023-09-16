#pragma once
#include <SDL.h>
#include <memory>

std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> init_window(uint screen_width, uint screen_height);

std::unique_ptr<void, void(*)(SDL_GLContext)> init_context(SDL_Window* window);
