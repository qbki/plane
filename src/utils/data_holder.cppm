module;
#include <SDL3/SDL_iostream.h>
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
  using RWopsPtrType = std::unique_ptr<SDL_IOStream,
                                       std::function<void(SDL_IOStream*)>>;

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


  SDL_IOStream*
  unsafe_rwops()
  {
    auto rw = SDL_IOFromConstMem(_data.data(), static_cast<int>(_data.size()));
    if (rw == nullptr) {
      pln::utils::crash("Can't create stream from raw data");
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
  rwops_deleter(SDL_IOStream* stream)
  {
    SDL_CloseIO(stream);
  }
};

}
