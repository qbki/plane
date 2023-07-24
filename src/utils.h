#pragma once
#include <SDL_events.h>
#include <string>
#include <tiny_gltf.h>

#include "./camera.h"

int buffer_size();

template<typename T>
void print(T& value);

tinygltf::Model load_gltf_model(const std::string& filename);

std::string load_text(const std::string &file_name);

void resize_window(
  const SDL_WindowEvent& window_event,
  Camera& camera
);
