#include <exception>
#include <string>

[[noreturn]] void
crash(const std::string& message);

[[noreturn]] void
crash(const std::exception& error);
