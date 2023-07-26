#pragma once
#include <memory>
#include <string>

#include "./graphic.h"
#include "./material.h"
#include "./mesh.h"
#include "./shader.h"

class Model : public Graphic {
private:
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Material> material;

public:
  using Graphic::Graphic;

  Model(
    std::shared_ptr<Mesh> mesh,
    std::shared_ptr<Shader> shader,
    std::shared_ptr<Material> material
  );

  ~Model() {};

  void draw(
    const Camera& camera,
    const SunLight& light,
    float elapsed_seconds
  ) const override;
};
