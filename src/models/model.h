#pragma once
#include <memory>
#include <string>

#include "../material.h"
#include "../mesh.h"
#include "../shader.h"
#include "../texture.h"
#include "graphic.h"

class Model : public Graphic {
private:
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Material> material;
  std::shared_ptr<Texture> texture;

public:
  using Graphic::Graphic;

  Model(
    std::shared_ptr<Mesh> mesh,
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Material> material,
    std::shared_ptr<Texture> texture
  );

  ~Model() {};

  void draw(
    const Camera& camera,
    const SunLight& light,
    float elapsed_seconds
  ) const override;
};
