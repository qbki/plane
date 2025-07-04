export module pln.utils.platform;

namespace pln::utils::platform {

#ifdef __EMSCRIPTEN__
  export constexpr bool IS_WEB { true };
  export constexpr bool IS_PC { false };
#else
  export constexpr bool IS_WEB { false };
  export constexpr bool IS_PC { true };
#endif

}
