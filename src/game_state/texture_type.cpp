#include <format>
#include <stdexcept>

#include "src/game_state/texture_type.h"
#include "src/services.h"

#include "texture_type.h"

namespace TextureType {

Type
handle_unknown_type()
{
  logger().warn("Unknown type of a texture");
  return DEFAULT_TYPE;
}

Type
handle_unknown_type(const std::string& message)
{
  logger().warn(std::format("Unknown type of a texture: {}", message));
  return DEFAULT_TYPE;
}

std::string
map_to_str(Type type)
{
  switch (type) {
    case Type::PRIMARY:
      return "primary";
    case Type::SECONDARY:
      return "secondary";
    default:
      throw std::out_of_range(
        "Unknown representation of texture type (string)");
  }
}

Type
map_to_type(const std::string& type)
{
  return (type == "primary"     ? Type::PRIMARY
          : type == "secondary" ? Type::SECONDARY
                                : handle_unknown_type(type));
}

unsigned int
map_to_id(Type type)
{
  return static_cast<unsigned int>(type);
}

Type
map_to_type(int type)
{
  switch (type) {
    case 0:
      return Type::PRIMARY;
    case 1:
      return Type::SECONDARY;
    default:
      return handle_unknown_type();
  }
}

unsigned int
map_str_to_id(const std::string& type)
{
  return map_to_id(map_to_type(type));
}
}
