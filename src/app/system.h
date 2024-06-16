#pragma once

struct System
{
#ifdef __EMSCRIPTEN__
  bool is_web = true;
  bool is_pc = false;
#else
  bool is_web = false;
  bool is_pc = true;
#endif
};
