#pragma once
#include <iostream>
#include "abstract_logger.h"


class Logger : public AbstractLogger {
public:
  void info(const std::string&) override;
  void warn(const std::string&) override;
  void error(const std::string&) override;
  void debug(const std::string&) override;
};
