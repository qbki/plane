#include "src/game_state/texture_type.h"

#include "textures.h"

Textures::Textures(TextureList textures)
  : _textures(std::move(textures))
  , _texture_type(TextureType::Type::MAIN)
{
}

void
Textures::change_type(TextureType::Type type)
{
  _texture_type = type;
}

const Texture&
Textures::texture() const
{
  return _textures->at(_texture_type);
}

TextureType::Type
Textures::texture_type() const
{
  return _texture_type;
}

void
Textures::use() const
{
  using TextureType::Type;
  _textures->at(Type::MAIN).use(0);
  _textures->at(Type::DESTROYED).use(1);
}
