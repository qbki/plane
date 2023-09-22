#include <exception>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "cache.h"
#include "../shader.h"
#include "../texture.h"
#include "../utils.h"


glm::vec3 exctract_material_color(tinygltf::Model& model) {
  auto material_id = model.meshes.at(0).primitives.at(0).material;
  auto material = model.materials.at(material_id);
  auto color = material.pbrMetallicRoughness.baseColorFactor;
  return glm::vec3(color.at(0), color.at(1), color.at(2));
}


std::vector<unsigned char> extract_texture(tinygltf::Model& model) {
  if (model.images.size() > 0) {
    return model.images.at(0).image;
  } else {
    auto color = exctract_material_color(model);
    return {
      static_cast<unsigned char>(color.x * 255.0),
      static_cast<unsigned char>(color.y * 255.0),
      static_cast<unsigned char>(color.z * 255.0),
      255
    };
  }
}


Cache::Cache() {
  this->meshes = std::unordered_map<
    std::string,
    std::tuple<mesh_ptr, material_ptr, texture_ptr>
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
  std::shared_ptr<Texture> texture;

  if (has_mesh) {
    auto mesh_data = this->meshes.at(mesh_file_name);
    mesh = std::get<MESH_IDX>(mesh_data);
    material = std::get<MATERIAL_IDX>(mesh_data);
    texture = std::get<TEXTURE_IDX>(mesh_data);
  } else {
    auto gltf_model = load_gltf_model(mesh_file_name);
    auto color = exctract_material_color(gltf_model);
    auto raw_texture = extract_texture(gltf_model);
    mesh = std::shared_ptr<Mesh>(new Mesh(gltf_model));
    material = std::shared_ptr<Material>(new Material(color));
    texture = std::shared_ptr<Texture>(new Texture(raw_texture));
    this->meshes[mesh_file_name] = std::make_tuple(mesh, material, texture);
  }

  return std::shared_ptr<Model>(new Model(mesh, shader, material, texture));
}
