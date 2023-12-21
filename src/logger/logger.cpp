#include <iostream>

#include "logger.h"

void
Logger::info(const std::string& message)
{
  std::cout << "[INFO] " << message << "\n";
}

void
Logger::warn(const std::string& message)
{
  std::cout << "[WARN] " << message << "\n";
}

void
Logger::error(const std::string& message)
{
  std::cerr << "[ERROR] " << message << std::endl;
}

void
Logger::debug(const std::string& message)
{
  std::cout << "[DEBUG] " << message << std::endl;
}
