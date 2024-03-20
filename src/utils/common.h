#pragma once
#include <cmath>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <limits>
#include <ostream>

template<typename M, typename K>
bool
has_key(const M& map, const K& key)
{
  return map.find(key) != map.end();
}

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
