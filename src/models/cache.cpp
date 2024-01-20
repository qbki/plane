#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "src/consts.h"
#include "src/game_state/texture_type.h"
#include "src/texture.h"
#include "src/utils/color.h"
#include "src/utils/common.h"
#include "src/utils/file_loaders.h"

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
  auto texture_id = TextureType::map_to_id(TextureType::DEFAULT_TYPE);
  auto textures =
    std::make_shared<std::unordered_map<TextureType::Type, Texture>>();
  textures->emplace(TextureType::DEFAULT_TYPE, Texture{ texture_id, data });
  return textures;
}

Cache::TexturesPtr
extract_textures(tinygltf::Model& model)
{
  auto textures =
    std::make_shared<std::unordered_map<TextureType::Type, Texture>>();
  for (const auto& image : model.images) {
    auto texture_type = TextureType::map_to_type(image.name);
    auto texture_id = TextureType::map_str_to_id(image.name);
    textures->emplace(texture_type, Texture{ texture_id, image.image });
  }
  return textures;
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
