module;
#include <filesystem>
#include <glm/vec3.hpp>
#include <optional>
#include <string>
#include <tiny_gltf.h>
#include <tuple>
#include <unordered_map>

export module pln.cache;

import pln.mesh;
import pln.mesh_generators;
import pln.services.app;
import pln.sounds.sound;
import pln.textures;
import pln.utils.color;
import pln.utils.file_loaders;

using namespace pln::utils::file_loaders;

namespace pln::cache {

export
class Cache
{
public:
  using MeshPtr = std::shared_ptr<pln::mesh::Mesh>;
  using TexturePtr = std::shared_ptr<pln::textures::Texture>;
  using ModelPair = std::tuple<MeshPtr, TexturePtr>;
  using SoundPtr = std::shared_ptr<pln::sounds::Sound>;

private:
  std::unordered_map<std::string, std::tuple<MeshPtr, TexturePtr>> _meshes;
  std::unordered_map<std::string, SoundPtr> _sounds;

public:
  Cache()
    : _meshes(std::unordered_map<std::string, std::tuple<MeshPtr, TexturePtr>>())
  {
  }


  Cache::ModelPair
  get_model(const std::filesystem::path& mesh_path)
  {
    if (_meshes.contains(mesh_path)) {
      return _meshes[mesh_path];
    }
    auto gltf_model = load_gltf_model(pln::services::app().assets_dir() / mesh_path)
                        .or_crash();
    auto extracted_texture_opt = extract_texture(gltf_model);
    auto texture = extracted_texture_opt.has_value()
                     ? extracted_texture_opt.value()
                     : generate_texture(gltf_model);
    auto mesh = std::make_shared<pln::mesh::Mesh>(gltf_model);
    _meshes[mesh_path] = std::make_tuple(mesh, texture);
    return { mesh, texture };
  }


  Cache::ModelPair
  get_rect(const pln::utils::color::Color& color)
  {
    auto rect_id = "#rect";
    if (_meshes.contains(rect_id)) {
      return _meshes[rect_id];
    }
    std::vector<unsigned char> data { color.r, color.g, color.b, color.a };
    auto texture = std::make_shared<pln::textures::Texture>(1, 1, data);
    auto mesh = std::shared_ptr(pln::mesh::create_quad());
    auto pair = std::make_tuple(mesh, texture);
    _meshes[rect_id] = pair;
    return pair;
  }


  Cache::SoundPtr
  get_sound(const std::filesystem::path& sound_path)
  {
    if (_sounds.contains(sound_path)) {
      return _sounds[sound_path];
    }
    std::shared_ptr<pln::sounds::Sound> sound = load_sound(sound_path);
    _sounds[sound_path] = sound;
    return sound;
  }

private:
  glm::vec3
  static exctract_material_color(tinygltf::Model& model)
  {
    if (model.materials.empty()) {
      return { 0, 0, 0 };
    }
    auto material_id = model.meshes.at(0).primitives.at(0).material;
    auto material = model.materials.at(material_id);
    auto color = material.pbrMetallicRoughness.baseColorFactor;
    return { color.at(0), color.at(1), color.at(2) };
  }


  static Cache::TexturePtr
  generate_texture(tinygltf::Model& model)
  {
    auto color = exctract_material_color(model);
    auto to_integer_value = [&](float value) {
      const float max_color_value = 255.0;
      return static_cast<unsigned char>(value * max_color_value);
    };
    std::vector<unsigned char> data { to_integer_value(color.x),
                                      to_integer_value(color.y),
                                      to_integer_value(color.z),
                                      to_integer_value(1.0) };
    return std::make_shared<pln::textures::Texture>(1, 1, data);
  }


  std::optional<Cache::TexturePtr>
  static extract_texture(const tinygltf::Model& model)
  {
    if (model.images.size() == 0) {
      return std::nullopt;
    }
    auto& image = model.images[0];
    auto texture = std::make_shared<pln::textures::Texture>(
      image.width, image.height, image.image);
    return std::make_optional(texture);
  }
};

}
