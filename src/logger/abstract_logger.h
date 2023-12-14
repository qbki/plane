#pragma once
#include <string>


class AbstractLogger {
public:
  virtual void info(const std::string&) = 0;
  virtual void warn(const std::string&) = 0;
  virtual void error(const std::string&) = 0;
  virtual void debug(const std::string&) = 0;
};
