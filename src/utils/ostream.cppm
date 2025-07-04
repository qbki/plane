module;
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <ostream>

export module pln.utils.ostream;

export
inline std::ostream&
operator<<(std::ostream& os, const glm::vec3& vec)
{
  return os << glm::to_string(vec);
}
