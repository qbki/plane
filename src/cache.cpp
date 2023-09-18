#include <exception>
#include <memory>
#include <stdexcept>
#include <tuple>

#include "./cache.h"
#include "./utils.h"
#include "material.h"
#include "shader.h"


Cache::Cache() {
  this->meshes = std::unordered_map<
    std::string,
    std::tuple<std::shared_ptr<Mesh>, std::shared_ptr<Material>>
  >();
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

  auto has_mesh = has_key(this->meshes, mesh_file_name);

  std::shared_ptr<Mesh> mesh;
  std::shared_ptr<Material> material;

  if (has_mesh) {
    mesh = std::get<MESH_IDX>(this->meshes.at(mesh_file_name));
    try {
      material = std::get<MATERIAL_IDX>(this->meshes.at(mesh_file_name));
    } catch (std::out_of_range error) {
      throw new std::runtime_error("Please ensure that " + mesh_file_name + " has a material");
    }
  } else {
    auto gltf_model = load_gltf_model(mesh_file_name);
    auto color = exctract_material_color(gltf_model);
    mesh = std::shared_ptr<Mesh>(new Mesh(gltf_model));
    material = std::shared_ptr<Material>(new Material(color));
    this->meshes[mesh_file_name] = std::make_tuple(mesh, material);
  }

  return std::shared_ptr<Model>(new Model(mesh, shader, material));
}
