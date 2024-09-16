#pragma once
#include <SDL_rwops.h>
#include <functional>
#include <memory>
#include <vector>

class DataHolder
{
private:
  std::vector<unsigned char> _data;
  static void rwops_deleter(SDL_RWops* rwops);

public:
  using RWopsPtrType = std::unique_ptr<SDL_RWops,
                                       std::function<void(SDL_RWops*)>>;

  explicit DataHolder(const std::vector<unsigned char>&& data);

  RWopsPtrType rwops();
  SDL_RWops* unsafe_rwops();
};
