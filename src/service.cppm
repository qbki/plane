module;
#include <memory>
#include <cassert>

export module pln.service;

namespace pln {


export
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


  static void
  install(std::unique_ptr<T> instance)
  {
    Service<T>::_instance = std::move(instance);
  }


  static T&
  get()
  {
    assert(_instance.get() && "An instance of Service was not found, you "
                              "should call the 'install' method first");
    return *Service<T>::_instance;
  }
};


template<typename T>
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::unique_ptr<T> Service<T>::_instance = std::unique_ptr<T>();

}
