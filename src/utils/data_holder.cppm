module;
#include <SDL_rwops.h>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

export module pln.utils.data_holder;

import pln.utils.crash;

namespace pln::utils::data_holder {

export class DataHolder
{
public:
  using RWopsPtrType = std::unique_ptr<SDL_RWops,
                                       std::function<void(SDL_RWops*)>>;

private:
  std::vector<char> _data;

public:
  explicit DataHolder(const std::vector<char>&& data)
    : _data(std::move(data))
  {
  }


  DataHolder::RWopsPtrType
  rwops()
  {
    return { unsafe_rwops(), rwops_deleter };
  }


  SDL_RWops*
  unsafe_rwops()
  {
    auto rw = SDL_RWFromConstMem(_data.data(), static_cast<int>(_data.size()));
    if (rw == nullptr) {
      pln::utils::crash("Can't create RWops");
    }
    return rw;
  }


  [[nodiscard]] const std::vector<char>&
  payload() const
  {
    return _data;
  }


private:
  static void
  rwops_deleter(SDL_RWops* rwops)
  {
    SDL_FreeRW(rwops);
  }
};

}
