#pragma once
#include <cxxabi.h>
#include <memory>
#include <optional>
#include <string>

#include "noop.h"

template<typename T>
using OptionalPtr = std::optional<std::unique_ptr<T>>;

template<typename T, typename TAG>
struct NewType
{
  T value;
  explicit NewType(T v)
    : value(v){};
};

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

// @link https://en.cppreference.com/w/cpp/utility/variant/visit
template<class... Ts>
struct Overloaded : Ts...
{
  using Ts::operator()...;
};
