module;
#include <iostream>
#include <string>

export module pln.logger.console_logger;

import pln.logger;

namespace pln::logger {

export
class ConsoleLogger : public pln::logger::AbstractLogger
{
public:
  ConsoleLogger() = default;
  ConsoleLogger(const ConsoleLogger&) = delete;
  ConsoleLogger& operator=(const ConsoleLogger&) = delete;
  ConsoleLogger(ConsoleLogger&&) = delete;
  ConsoleLogger& operator=(ConsoleLogger&&) = delete;
  ~ConsoleLogger() override = default;

  void
  info(const std::string& message) override
  {
    std::cout << "[INFO] " << message << "\n";
  }

  void
  warn(const std::string& message) override
  {
    std::cout << "[WARN] " << message << "\n";
  }

  void
  error(const std::string& message) override
  {
    std::cerr << "[ERROR] " << message << std::endl;
  }

  void
  debug(const std::string& message) override
  {
    std::cout << "[DEBUG] " << message << std::endl;
  }
};

}
