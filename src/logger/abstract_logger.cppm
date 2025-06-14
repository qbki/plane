module;
#include <string>

export module pln.logger;

namespace pln::logger {

export class AbstractLogger
{
public:
  AbstractLogger() = default;
  AbstractLogger(const AbstractLogger&) = delete;
  AbstractLogger& operator=(const AbstractLogger&) = delete;
  AbstractLogger(AbstractLogger&&) = delete;
  AbstractLogger& operator=(AbstractLogger&&) = delete;
  virtual ~AbstractLogger() = default;

  virtual void info(const std::string&) = 0;
  virtual void warn(const std::string&) = 0;
  virtual void error(const std::string&) = 0;
  virtual void debug(const std::string&) = 0;
};

}
