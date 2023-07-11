#include <memory>
#include <string>

#include "mesh.h"
#include "shader.h"

class Model {
private:
  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Shader> shader;

public:
  Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader);
  void draw();
  static std::unique_ptr<Model> load(
    const std::string& mesh_file_name,
    const std::string& vertex_shader_file_name,
    const std::string& fragment_shader_file_name);
};
