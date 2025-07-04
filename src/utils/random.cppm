module;
#include <functional>
#include <memory>
#include <random>
#include <type_traits>

export module pln.utils.random;

namespace pln::utils {

export
template<typename T>
concept is_number = std::is_floating_point_v<T> || std::is_integral_v<T>;

export
template<typename T>
  requires is_number<T>
decltype(auto) constexpr get_distribution(T start, T end)
{
  if constexpr (std::is_floating_point<T>()) {
    using Distribution = std::uniform_real_distribution<T>;
    return std::make_shared<Distribution>(start, end);
  } else {
    using Distribution = std::uniform_int_distribution<T>;
    return std::make_shared<Distribution>(start, end);
  }
}

export
template<typename T>
  requires is_number<T>
std::function<T()>
make_random_fn(T start, T end)
{
  auto random_device = std::make_shared<std::random_device>();
  auto distribution = get_distribution(start, end);
  return [=]() { return (*distribution)(*random_device); };
}

}
