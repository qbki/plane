#pragma once
#include <SDL_events.h>
#include <cmath>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <limits>
#include <ostream>
#include <string>
#include <unordered_map>
#include <tiny_gltf.h>

#include "camera.h"

int buffer_size();

template<typename T>
T zero();

template<>
inline glm::vec3 zero<glm::vec3>() {
  return glm::vec3(0.0, 0.0, 0.0);
}

template<typename T>
void print(T& value);


tinygltf::Model load_gltf_model(const std::string& filename);


std::string load_text(const std::string &file_name);


void resize_window(
  const SDL_WindowEvent& window_event,
  Camera& camera
);


template<typename M, typename K>
bool has_key(const M& map, const K& key) {
  return map.find(key) != map.end();
}


template<typename T>
bool is_approx_equal(T a, T b) {
  return std::fabs(b - a) < std::numeric_limits<T>::epsilon();
}


inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
  return os << glm::to_string(vec);
}
