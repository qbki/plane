module;
#define GLM_ENABLE_EXPERIMENTAL
#include <cmath>
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <limits>
#include <vector>

export module pln.utils.common;

namespace pln::utils::common {


export
template<typename T>
bool
is_approx_equal(T a, T b)
{
  return std::fabs(b - a) < std::numeric_limits<T>::epsilon();
}


export
std::vector<unsigned char>
get_pixels(int width, int height, unsigned int size = 4)
{
  std::vector<unsigned char> result;
  result.resize(width * height * size, 0);
  return result;
}


}
