#pragma once
#include <memory>
#include <unordered_map>

#include "src/game_state/texture_type.h"
#include "src/texture.h"

class Textures
{
private:
  using TextureList =
    std::shared_ptr<const std::unordered_map<TextureType::Type, Texture>>;
  TextureList _textures;
  const Texture* _selected_texture = nullptr;

public:
  explicit Textures(TextureList textures);

  void change_type(TextureType::Type type);
  [[nodiscard]] const Texture& texture() const;
};
