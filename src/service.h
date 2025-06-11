#pragma once
#include <format>
#include <memory>

#include "src/components/common.h"
#include "utils/types.h"

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
      crash(std::format("An instance of {} was not found, you should call the "
                        "'install' method first",
                        demangled_name<T>()));
    }
    return *Service<T>::_instance.get();
  }
};

template<typename T>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::unique_ptr<T> Service<T>::_instance = std::unique_ptr<T>();
