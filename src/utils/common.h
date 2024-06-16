#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <cmath>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <limits>
#include <ostream>
#include <string>
#include <vector>

template<typename T>
bool
is_approx_equal(T a, T b)
{
  return std::fabs(b - a) < std::numeric_limits<T>::epsilon();
}

inline std::ostream&
operator<<(std::ostream& os, const glm::vec3& vec)
{
  return os << glm::to_string(vec);
}

std::vector<unsigned char>
get_pixels(int width, int height, unsigned int size = 4);
