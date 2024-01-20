#include "textures.h"

Textures::Textures(TextureList textures)
  : _textures(std::move(textures))
{
  change_type(TextureType::Type::MAIN);
}

void
Textures::change_type(TextureType::Type type)
{
  _selected_texture = &_textures->at(type);
}

const Texture&
Textures::texture() const
{
  return *_selected_texture;
}
