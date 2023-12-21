#pragma once
#include <format>
#include <memory>
#include <stdexcept>

#include "logger/abstract_logger.h"
#include "utils.h"

template<typename T>
class Service
{
private:
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
  static std::unique_ptr<T> _instance;

public:
  Service() = delete;
  Service(const Service&) = delete;
  Service& operator=(const Service&) = delete;
  Service(Service&&) = delete;
  Service& operator=(Service&&) = delete;
  ~Service() = delete;

  static void install(std::unique_ptr<T> instance)
  {
    Service<T>::_instance = std::move(instance);
  }

  static T& get()
  {
    if (!_instance) {
      throw std::runtime_error(
        std::format("An instance of {} was not found, you should call the "
                    "'install' method first",
                    demangled_name<T>()));
    }
    return *Service<T>::_instance.get();
  }
};

template<typename T>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::unique_ptr<T> Service<T>::_instance = std::unique_ptr<T>();
