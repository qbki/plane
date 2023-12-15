#pragma once
#include <GL/glew.h>
#include <SDL_events.h>
#include <cxxabi.h>
#include <format>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <tiny_gltf.h>

#include "camera.h"


inline void noop() {};


int buffer_size();


template<typename T>
T zero();


template<>
inline glm::vec3 zero<glm::vec3>() {
  return glm::vec3(0.0, 0.0, 0.0);
}


/**
 * HACK: emscripten's libc++ doesn't support custom types for std::format
 * well. Therefore, you should use this function for the GLubyte* type.
 * Use this after emscripten updates libc++:
 *
 * template <>
 * struct std::formatter<const GLubyte*> : std::formatter<std::string> {
 *   auto format(const GLubyte* text, format_context& ctx) const {
 *     auto reinterpreted_text = reinterpret_cast<const char*>(text);
 *     return formatter<string>::format(
 *       std::format("{}", reinterpreted_text),
 *       ctx
 *     );
 *   }
 * };
 */
std::string glubyte_to_string(const GLubyte* value);


glm::vec3 calc_z_direction(const glm::vec3& rotation);


tinygltf::Model load_gltf_model(const std::string& filename);


std::string load_text(const std::string &file_name);


glm::vec3 encode_gamma(const glm::vec3& color, float gamma);


glm::vec3 decode_gamma(const glm::vec3& color, float gamma);


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


glm::mat4 make_transform_matrix(glm::vec3 position, glm::vec3 rotation);


template<typename T, typename TAG>
struct NewType {
  T value;
  explicit NewType(T v) : value(v) {};
};


template<typename TAG>
struct EmptyType {
  char value = 'e';
  explicit EmptyType() {};
};


template<typename T>
using OptionalPtr = std::optional<std::unique_ptr<T>>;


/**
 * y = value * smoothing ^ dt
 * value - interpolated value
 * smoothing - rate of decay
 * example:
 *   dt | value = 10 and smoothing = 0.5
 *   ---|-------------------------------
 *   0  | 10
 *   1  | 10 / 2
 *   2  | 10 / 4
 */
template<typename T>
T damp(T from, T to, float smoothing, float dt) {
  return to + (from - to) * std::pow(smoothing, dt);
}


void print_opengl_errors();


void print_opengl_info();


void print_extension_support(std::string extension_name);


template<typename T>
std::string demangled_name() {
  int status;
  auto name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
  std::string result{name};
  std::free(name);
  switch (status) {
    case 0:
      noop();
      break;
    case -1:
      throw std::runtime_error("Demangle. A memory allocation failure occurred.");
    case -2:
      throw std::runtime_error("Demangle. A mangled_name is not a valid name under the C++ ABI mangling rules.");
    case -3:
      throw std::runtime_error("Demangle. One of the arguments is invalid.");
    default:
      throw std::runtime_error("Demangle. Unknown error.");
  }
  return result;
}
