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
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<Material> _material;
  std::vector<std::shared_ptr<Texture>> _textures;
  unsigned int _basecolor_texture_idx = 0;

public:
  using Graphic::Graphic;

  Model(
    std::shared_ptr<Mesh> mesh,
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Material> material,
    std::vector<std::shared_ptr<Texture>> textures
  );

  ~Model() {};

  void draw(
    const Camera& camera,
    const SunLight& light,
    float elapsed_seconds
  ) const override;

  void use_basecolor_texture(Texture::Type type);
};
