#pragma once
#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <ostream>

glm::vec3
calc_z_direction(const glm::vec3& rotation);

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

glm::mat4
make_transform_matrix(glm::vec3 position, glm::vec3 rotation);
