#include <memory>

#include "./cache.h"
#include "./utils.h"
#include "shader.h"


Cache::Cache() {
  this->materials = std::unordered_map<std::string, std::shared_ptr<Material>>();
  this->meshes = std::unordered_map<std::string, std::shared_ptr<Mesh>>();
  this->shaders = std::unordered_map<std::string, std::shared_ptr<Shader>>();
}


std::shared_ptr<Model> Cache::load(
  const std::string& mesh_file_name,
  const std::string& vertex_shader_file_name,
  const std::string& fragment_shader_file_name
) {
  auto shader_file_names = vertex_shader_file_name + fragment_shader_file_name;
  auto has_shader = has_key(this->shaders, shader_file_names);
  auto shader = has_shader
    ? this->shaders.at(shader_file_names)
    : std::shared_ptr<Shader>(new Shader());
  if (!has_shader) {
    auto vertex_shader = load_text(vertex_shader_file_name);
    auto fragment_shader = load_text(fragment_shader_file_name);
    shader->compile(vertex_shader, fragment_shader);
    this->shaders[shader_file_names] = shader;
  }

  auto gltf_model = load_gltf_model(mesh_file_name);
  auto has_mesh = has_key(this->meshes, mesh_file_name);
  std::shared_ptr<Mesh> mesh = has_shader
    ? this->meshes.at(mesh_file_name)
    : std::shared_ptr<Mesh>(new Mesh(gltf_model));
  if (!has_mesh) {
    this->meshes[mesh_file_name] = mesh;
  }

  auto color = exctract_material_color(gltf_model);
  auto has_material = has_key(this->materials, mesh_file_name);
  auto material = has_material
    ? this->materials.at(mesh_file_name)
    : std::shared_ptr<Material>(new Material(color));
  if (!has_material) {
    this->materials[mesh_file_name] = material;
  }

  return std::shared_ptr<Model>(new Model(mesh, shader, material));
}
