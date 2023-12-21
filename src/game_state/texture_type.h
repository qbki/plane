#pragma once
#include <string>

namespace TextureType {
enum class Type : unsigned int
{
  MAIN,
  DESTROYED,
};

inline const Type DEFAULT_TYPE = Type::MAIN;

std::string
map_to_str(Type type);
Type
map_to_type(const std::string& type);

unsigned int
map_to_int(Type type);
Type
map_to_type(int type);

unsigned int
map_str_to_int(const std::string& type);
};
