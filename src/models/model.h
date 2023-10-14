#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../material.h"
#include "../mesh.h"
#include "../shader.h"
#include "../texture.h"
#include "graphic.h"

class Model : public Graphic {
private:
  std::shared_ptr<Mesh> _mesh;
  std::vector<std::shared_ptr<Texture>> _textures;
  unsigned int _basecolor_texture_idx = 0;

public:
  using Graphic::Graphic;

  Model(
    std::shared_ptr<Mesh> mesh,
    std::vector<std::shared_ptr<Texture>> textures);

  ~Model() {};

  void draw(std::function<void(Graphic&)>& fn) override;

  void use_basecolor_texture(unsigned int type);
  std::shared_ptr<Texture> texture();
};
