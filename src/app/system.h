#pragma once

struct System
{
#ifdef __EMSCRIPTEN__
  static constexpr bool is_web = true;
  static constexpr bool is_pc = false;
#else
  static constexpr bool is_web = false;
  static constexpr bool is_pc = true;
#endif
};
