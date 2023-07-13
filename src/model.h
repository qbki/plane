#pragma once
#include <memory>
#include <string>

#include "graphic.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"

class Model : public Graphic {
private:
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Shader> shader;

public:
  using Graphic::Graphic;

  Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader);

  void draw(const Camera& camera);

  static std::unique_ptr<Model> load(
    const std::string& mesh_file_name,
    const std::string& vertex_shader_file_name,
    const std::string& fragment_shader_file_name);
};
