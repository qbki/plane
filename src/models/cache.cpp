#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "../consts.h"
#include "../game_state/index.h"
#include "../shader.h"
#include "../texture.h"
#include "../utils.h"
#include "cache.h"

glm::vec3
exctract_material_color(tinygltf::Model& model)
{
  auto material_id = model.meshes.at(0).primitives.at(0).material;
  auto material = model.materials.at(material_id);
  auto color = material.pbrMetallicRoughness.baseColorFactor;
  return { color.at(0), color.at(1), color.at(2) };
}

Cache::TexturesPtr
generate_texture(tinygltf::Model& model)
{
  // Gamma will be calculated in shader
  auto color = exctract_material_color(model);
  auto to_integer_value = [&](float value) {
    const float max_color_value{ 255.0 };
    return static_cast<unsigned char>(value * max_color_value);
  };
  std::vector<unsigned char> data{ to_integer_value(color.x),
                                   to_integer_value(color.y),
                                   to_integer_value(color.z),
                                   to_integer_value(1.0) };
  auto texture_type = TextureType::map_to_int(TextureType::DEFAULT_TYPE);
  Cache::TexturesPtr textures =
    std::make_shared<std::vector<Texture>>(std::vector<Texture>());
  textures->emplace_back(texture_type, data);
  return textures;
}

Cache::TexturesPtr
extract_textures(tinygltf::Model& model)
{
  auto textures = std::vector<Texture>();
  std::ranges::transform(
    model.images,
    std::back_inserter(textures),
    [](const tinygltf::Image& image) {
      return Texture{ TextureType::map_str_to_int(image.name), image.image };
    });
  return std::make_shared<std::vector<Texture>>(std::move(textures));
}

Cache::Cache()
{
  this->_meshes =
    std::unordered_map<std::string,
                       std::tuple<MeshPtr, MaterialPtr, TexturesPtr>>();
}

std::tuple<Cache::MeshPtr, Cache::TexturesPtr>
Cache::load(const std::string& mesh_file_name)
{
  auto has_mesh = has_key(this->_meshes, mesh_file_name);

  Cache::MeshPtr mesh;
  Cache::MaterialPtr material;
  TexturesPtr textures;

  if (has_mesh) {
    auto mesh_data = this->_meshes.at(mesh_file_name);
    mesh = std::get<MESH_IDX>(mesh_data);
    textures = std::get<TEXTURE_IDX>(mesh_data);
  } else {
    auto gltf_model = load_gltf_model(mesh_file_name);
    auto color = decode_gamma(exctract_material_color(gltf_model), GAMMA);

    mesh = std::make_shared<Mesh>(gltf_model);
    material = std::make_shared<Material>(color);
    auto extracted_textures = extract_textures(gltf_model);
    textures = extracted_textures->size() > 0 ? extracted_textures
                                              : generate_texture(gltf_model);
    this->_meshes[mesh_file_name] = std::make_tuple(mesh, material, textures);
  }

  return { mesh, textures };
}
