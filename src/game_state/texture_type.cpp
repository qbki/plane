#include <stdexcept>

#include "../services.h"
#include "texture_type.h"

namespace TextureType {
Type
handle_unknown_type()
{
  logger().warn("Unknown type of a texture");
  return DEFAULT_TYPE;
}

std::string
map_to_str(Type type)
{
  switch (type) {
    case Type::MAIN:
      return "main";
    case Type::DESTROYED:
      return "destroyed";
    default:
      throw std::out_of_range(
        "Unknown representation of texture type (string)");
  }
}

Type
map_to_type(const std::string& type)
{
  return (type == "main"        ? Type::MAIN
          : type == "destroyed" ? Type::DESTROYED
                                : handle_unknown_type());
}

unsigned int
map_to_int(Type type)
{
  return static_cast<unsigned int>(type);
}

Type
map_to_type(int type)
{
  switch (type) {
    case 0:
      return Type::MAIN;
    case 1:
      return Type::DESTROYED;
    default:
      return handle_unknown_type();
  }
}

unsigned int
map_str_to_int(const std::string& type)
{
  return map_to_int(map_to_type(type));
}
}
