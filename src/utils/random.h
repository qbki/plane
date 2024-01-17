#pragma once
#include <functional>
#include <memory>
#include <random>

template<typename T>
std::function<T()>
make_random_fn(T start, T end)
{
  using Distribution = std::uniform_int_distribution<T>;
  auto random_device = std::make_shared<std::random_device>();
  auto distribution = std::make_shared<Distribution>(start, end);
  return [=]() { return (*distribution)(*random_device); };
}
