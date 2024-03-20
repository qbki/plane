#include <filesystem>
#include <glm/vec3.hpp>
#include <tiny_gltf.h>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "src/consts.h"
#include "src/game_state/texture_type.h"
#include "src/texture.h"
#include "src/utils/common.h"
#include "src/utils/file_loaders.h"

#include "cache.h"

glm::vec3
exctract_material_color(tinygltf::Model& model)
{
  if (model.materials.empty()) {
    return { 0, 0, 0 };
  }
  auto material_id = model.meshes.at(0).primitives.at(0).material;
  auto material = model.materials.at(material_id);
  auto color = material.pbrMetallicRoughness.baseColorFactor;
  return { color.at(0), color.at(1), color.at(2) };
}

Cache::TexturesPtr
generate_textures(tinygltf::Model& model)
{
  auto color = exctract_material_color(model);
  auto to_integer_value = [&](float value) {
    const float max_color_value{ 255.0 };
    return static_cast<unsigned char>(value * max_color_value);
  };
  std::vector<unsigned char> data{ to_integer_value(color.x),
                                   to_integer_value(color.y),
                                   to_integer_value(color.z),
                                   to_integer_value(1.0) };
  auto textures =
    std::make_shared<std::unordered_map<TextureType::Type, Texture>>();
  textures->emplace(TextureType::Type::PRIMARY, Texture{ data });
  textures->emplace(TextureType::Type::SECONDARY, Texture{ data });
  return textures;
}

Cache::TexturesPtr
extract_textures(const tinygltf::Model& model)
{
  auto textures =
    std::make_shared<std::unordered_map<TextureType::Type, Texture>>();
  for (const auto& image : model.images) {
    auto texture_type = TextureType::map_to_type(image.name);
    textures->emplace(texture_type, Texture{ image.image });
  }
  return textures;
}

Cache::Cache()
{
  this->_meshes =
    std::unordered_map<std::string, std::tuple<MeshPtr, TexturesPtr>>();
}

std::tuple<Cache::MeshPtr, Cache::TexturesPtr>
Cache::load(const std::string& mesh_file_name)
{
  auto has_mesh = has_key(this->_meshes, mesh_file_name);

  Cache::MeshPtr mesh;
  TexturesPtr textures;

  if (has_mesh) {
    auto mesh_data = this->_meshes.at(mesh_file_name);
    mesh = std::get<MESH_IDX>(mesh_data);
    textures = std::get<TEXTURE_IDX>(mesh_data);
  } else {
    auto gltf_model = load_gltf_model(ASSETS_DIR / mesh_file_name);
    auto extracted_textures = extract_textures(gltf_model);

    mesh = std::make_shared<Mesh>(gltf_model);
    textures = generate_textures(gltf_model);
    for (auto& [idx, texture] : *extracted_textures) {
      textures->at(idx) = std::move(texture);
    }
    this->_meshes[mesh_file_name] = std::make_tuple(mesh, textures);
  }

  return { mesh, textures };
}
