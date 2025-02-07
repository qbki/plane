#include <SDL_rwops.h>
#include <utility>

#include "src/utils/crash.h"

#include "data_holder.h"

DataHolder::DataHolder(const std::vector<unsigned char>&& data)
  : _data(std::move(data))
{
}

void
DataHolder::rwops_deleter(SDL_RWops* rwops)
{
  SDL_FreeRW(rwops);
}

DataHolder::RWopsPtrType
DataHolder::rwops()
{
  return { unsafe_rwops(), rwops_deleter };
}

SDL_RWops*
DataHolder::unsafe_rwops()
{
  auto rw = SDL_RWFromConstMem(_data.data(), static_cast<int>(_data.size()));
  if (rw == nullptr) {
    crash("Can't create RWops");
  }
  return rw;
}
