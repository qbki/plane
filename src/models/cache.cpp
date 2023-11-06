#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "../game_state/index.h"
#include "../shader.h"
#include "../texture.h"
#include "../utils.h"
#include "../consts.h"
#include "cache.h"


glm::vec3 exctract_material_color(tinygltf::Model& model) {
  auto material_id = model.meshes.at(0).primitives.at(0).material;
  auto material = model.materials.at(material_id);
  auto color = material.pbrMetallicRoughness.baseColorFactor;
  return glm::vec3(color.at(0), color.at(1), color.at(2));
}


Cache::TexturesPtr generate_texture(tinygltf::Model& model) {
  // Gamma will be calculated in shader
  auto color = exctract_material_color(model);
  std::vector<unsigned char> data {
    static_cast<unsigned char>(color.x * 255.0),
    static_cast<unsigned char>(color.y * 255.0),
    static_cast<unsigned char>(color.z * 255.0),
    255
  };
  auto texture_type = TextureType::map_to_int(TextureType::DEFAULT_TYPE);
  Cache::TexturesPtr textures = std::make_shared<std::vector<Texture>>(std::vector<Texture>());
  textures->emplace_back(texture_type, data);
  return textures;
}


Cache::TexturesPtr extract_textures(tinygltf::Model& model) {
  auto textures = std::vector<Texture>();
  textures.reserve(model.images.size());
  for (auto& image : model.images) {
    textures.emplace_back(TextureType::map_str_to_int(image.name), image.image);
  }
  return std::make_shared<std::vector<Texture>>(std::move(textures));
}


Cache::Cache() {
  this->_meshes = std::unordered_map<
    std::string,
    std::tuple<MeshPtr, MaterialPtr, TexturesPtr>
  >();
}


std::tuple<Cache::MeshPtr, Cache::TexturesPtr>
Cache::load(const std::string& mesh_file_name) {
  auto has_mesh = has_key(this->_meshes, mesh_file_name);

  Cache::MeshPtr mesh;
  Cache::MaterialPtr material;
  TexturesPtr textures;

  if (has_mesh) {
    auto mesh_data = this->_meshes.at(mesh_file_name);
    mesh = std::get<MESH_IDX>(mesh_data);
    material = std::get<MATERIAL_IDX>(mesh_data);
    textures = std::get<TEXTURE_IDX>(mesh_data);
  } else {
    auto gltf_model = load_gltf_model(mesh_file_name);
    auto color = decode_gamma(exctract_material_color(gltf_model), GAMMA);

    mesh = std::make_shared<Mesh>(gltf_model);
    material = std::make_shared<Material>(color);
    auto extracted_textures = extract_textures(gltf_model);
    textures = extracted_textures->size() > 0
      ? extracted_textures
      : generate_texture(gltf_model);
    this->_meshes[mesh_file_name] = std::make_tuple(mesh, material, textures);
  }

  return {mesh, textures};
}
