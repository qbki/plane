#include <algorithm>
#include <filesystem>
#include <memory>

#include "./model.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"


Model::Model(
  std::shared_ptr<Mesh> _mesh,
  std::shared_ptr<Shader> _shader
) : mesh(_mesh),
    shader(_shader) {}


void Model::draw() {
  shader->use();
  mesh->draw();
}


std::unique_ptr<Model> Model::load(
  const std::string& mesh_file_name,
  const std::string& vertex_shader_file_name,
  const std::string& fragment_shader_file_name
) {
  auto vertex_shader = load_text(vertex_shader_file_name);
  auto fragment_shader = load_text(fragment_shader_file_name);

  std::shared_ptr<Shader> shader(new Shader);
  shader->load(vertex_shader, fragment_shader);

  auto raw_mesh = load_mesh(mesh_file_name);
  std::shared_ptr<Mesh> mesh(new Mesh(raw_mesh));

  return std::unique_ptr<Model>(new Model(mesh, shader));
}
