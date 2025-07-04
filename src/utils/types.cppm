module;
#include <SDL_rwops.h>
#include <cxxabi.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <optional>
#include <string>

export module pln.utils.types;

import pln.utils.common;
import pln.utils.crash;

namespace pln::utils {

export
template<typename T>
using OptionalPtr = std::optional<std::unique_ptr<T>>;


export
template<typename T, typename TAG>
struct NewType
{
  T value;
  explicit NewType(T v)
    : value(std::move(v)) {};
};


export
template<typename T>
std::string
demangled_name()
{
  int status = 0;
  auto name = std::make_unique<char*>(
    abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status));
  std::string result { *name };
  switch (status) {
    case 0:
      pln::utils::common::noop();
      break;
    case -1:
      pln::utils::crash("Demangle. A memory allocation failure occurred.");
    case -2:
      pln::utils::crash("Demangle. A mangled_name is not a valid name "
                        "under the C++ ABI mangling rules.");
    case -3:
      pln::utils::crash("Demangle. One of the arguments is invalid.");
    default:
      pln::utils::crash("Demangle. Unknown error.");
  }
  return result;
}


// @link https://en.cppreference.com/w/cpp/utility/variant/visit
export
template<class... Ts>
struct Overloaded : Ts...
{
  using Ts::operator()...;
};

}
