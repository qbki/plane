#pragma once
#include <string>

namespace TextureType {
enum class Type : unsigned char
{
  PRIMARY = 0,
  SECONDARY = 1,
};

inline const Type DEFAULT_TYPE = Type::PRIMARY;

std::string
map_to_str(Type type);
Type
map_to_type(const std::string& type);

unsigned int
map_to_id(Type type);
Type
map_to_type(int type);

unsigned int
map_str_to_id(const std::string& type);
};
