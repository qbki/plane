module;
#include <SDL_rwops.h>
#include <glm/vec3.hpp>
#include <limits>
#include <vector>

export module utils.color;

namespace utils::color {

export struct Color
{
  constexpr static auto MAX_COLOR { std::numeric_limits<std::uint8_t>::max() };
  constexpr static auto ONE_BYTE { 8 };
  constexpr static auto TWO_BYTES { 16 };
  constexpr static auto THREE_BYTES { 24 };

  std::uint8_t r { MAX_COLOR };
  std::uint8_t g { MAX_COLOR };
  std::uint8_t b { MAX_COLOR };
  std::uint8_t a { MAX_COLOR };

  // cppcheck-suppress[functionStatic]
  constexpr explicit Color(std::uint32_t _value)
    : r((_value & (MAX_COLOR << THREE_BYTES)) >> THREE_BYTES)
    , g((_value & (MAX_COLOR << TWO_BYTES)) >> TWO_BYTES)
    , b((_value & (MAX_COLOR << ONE_BYTE)) >> ONE_BYTE)
    , a(_value & MAX_COLOR)
  {
  }

  // cppcheck-suppress[functionStatic]
  constexpr explicit Color(glm::vec3 value)
    : r(denorm(value.r))
    , g(denorm(value.g))
    , b(denorm(value.b))
  {
  }

  // cppcheck-suppress[functionStatic]
  constexpr Color(std::uint8_t _r,
                  std::uint8_t _g,
                  std::uint8_t _b,
                  std::uint8_t _a = std::numeric_limits<std::uint8_t>::max())
    : r(_r)
    , g(_g)
    , b(_b)
    , a(_a)
  {
  }

  constexpr Color& operator=(glm::vec3 other)
  {
    this->r = denorm(other.r);
    this->g = denorm(other.g);
    this->b = denorm(other.b);
    return *this;
  }

  constexpr bool operator==(const Color& other) const
  {
    return a == other.a && r == other.r && g == other.g && b == other.b;
  }

  [[nodiscard]] std::vector<std::uint8_t> constexpr to_array() const
  {
    return { r, g, b, a };
  }

  constexpr static std::uint8_t denorm(float value)
  {
    return static_cast<uint8_t>(value * MAX_COLOR);
  }
};

}
