module;
#include <cstdlib>
#include <functional>
#include <memory>
#include <variant>

export module pln.utils.result;

import pln.utils.crash;
import pln.utils.types;

namespace pln::utils {

/**
 * TODO Should be replaced by std::expected (C++23)
 */
export
template<typename T>
class Result
{
public:
  using type = std::variant<T, std::shared_ptr<std::exception>>;

private:
  constexpr static std::size_t PAYLOAD_IDX = 0;
  constexpr static std::size_t ERROR_IDX = 1;

  type _variant;

public:
  static Result<T> from_payload(const T& payload)
  {
    auto value = Result<T>();
    value._variant.template emplace<PAYLOAD_IDX>(payload);
    return value;
  }

  static Result<T> from_error(std::shared_ptr<std::exception> error)
  {
    auto value = Result<T>();
    value._variant.template emplace<ERROR_IDX>(error);
    return value;
  }

  template<typename R>
  Result<R> then(const std::function<Result<R>(T&)>& fn)
  {
    return std::visit(Overloaded {
                        [&fn](T& value) { return fn(value); },
                        [](const std::shared_ptr<std::exception>& error) {
                          return Result<R>::from_error(error);
                        },
                      },
                      _variant);
  }

  T or_crash()
  {
    if (std::holds_alternative<T>(_variant)) {
      return std::get<PAYLOAD_IDX>(_variant);
    }
    pln::utils::crash(*std::get<ERROR_IDX>(_variant));
  }

  bool has_payload() { return std::holds_alternative<T>(_variant); }

  T payload() { return std::get<PAYLOAD_IDX>(_variant); }

  std::shared_ptr<std::exception> error()
  {
    return std::get<ERROR_IDX>(_variant);
  }

  T or_fallback(const T& fallback)
  {
    return std::visit(
      Overloaded {
        [&](const T& value) -> T { return value; },
        [&](const std::shared_ptr<std::exception>&) -> T { return fallback; },
      },
      _variant);
  }
};

}
