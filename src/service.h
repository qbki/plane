#pragma once
#include <memory>
#include <stdexcept>
#include <format>

#include "logger/abstract_logger.h"
#include "utils.h"


template<typename T>
class Service {
public:
  Service() = delete;
  Service(const Service&) = delete;
  Service& operator=(const Service&) = delete;

  static std::unique_ptr<T> _instance;


  static void install(std::unique_ptr<T> instance) {
    Service<T>::_instance = std::move(instance);
  }


  static T& get() {
    if (!_instance) {
      throw std::runtime_error(std::format(
        "An instance of {} was not found, you should call the 'install' method first",
        demangled_name<T>()
      ));
    }
    return *Service<T>::_instance.get();
  }
};

template<typename T>
std::unique_ptr<T> Service<T>::_instance = std::unique_ptr<T>();
