#pragma once
#include "abstract_logger.h"

class Logger : public AbstractLogger
{
public:
  Logger() = default;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
  ~Logger() override = default;

  void info(const std::string&) override;
  void warn(const std::string&) override;
  void error(const std::string&) override;
  void debug(const std::string&) override;
};
