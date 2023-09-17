#pragma once
#include <SDL_events.h>
#include <cmath>
#include <limits>
#include <string>
#include <unordered_map>
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

glm::vec3 exctract_material_color(tinygltf::Model& model);

template<typename K, typename V>
bool has_key(const std::unordered_map<K, V>& map, const K& key) {
  return map.find(key) != map.end();
}

template<typename T>
bool is_approx_equal(T a, T b) {
  return std::fabs(b - a) < std::numeric_limits<T>::epsilon();
}
