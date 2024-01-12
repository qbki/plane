#include <cxxabi.h>
#include <memory>
#include <string>

#include "noop.h"

template<typename T>
std::string
demangled_name()
{
  int status = 0;
  auto name = std::make_unique<char*>(
    abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status));
  std::string result{ *name };
  switch (status) {
    case 0:
      noop();
      break;
    case -1:
      throw std::runtime_error(
        "Demangle. A memory allocation failure occurred.");
    case -2:
      throw std::runtime_error("Demangle. A mangled_name is not a valid name "
                               "under the C++ ABI mangling rules.");
    case -3:
      throw std::runtime_error("Demangle. One of the arguments is invalid.");
    default:
      throw std::runtime_error("Demangle. Unknown error.");
  }
  return result;
}
