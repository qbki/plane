#include <filesystem>
#include <glm/vec3.hpp>
#include <optional>
#include <string>
#include <tiny_gltf.h>
#include <tuple>
#include <unordered_map>

#include "src/services.h"
#include "src/sound/sound.h"
#include "src/texture.h"
#include "src/utils/file_loaders.h"
#include "src/utils/result.h"

#include "src/cache/cache.h"

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

Cache::TexturePtr
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
  return std::make_shared<Texture>(1, 1, data);
}

std::optional<Cache::TexturePtr>
extract_texture(const tinygltf::Model& model)
{
  if (model.images.size() == 0) {
    return std::nullopt;
  }
  auto& image = model.images[0];
  auto texture = std::make_shared<Texture>(
    image.width, image.height, image.image);
  return std::make_optional(texture);
}

Cache::Cache()
  : _meshes(std::unordered_map<std::string, std::tuple<MeshPtr, TexturePtr>>())
{
}

Cache::ModelPair
Cache::get_model(const std::filesystem::path& mesh_path)
{
  if (_meshes.contains(mesh_path)) {
    return _meshes[mesh_path];
  }
  auto gltf_model = load_gltf_model(Services::app().assets_dir() / mesh_path)
                      .or_crash();
  auto extracted_texture_opt = extract_texture(gltf_model);
  auto texture = extracted_texture_opt.has_value()
                   ? extracted_texture_opt.value()
                   : generate_texture(gltf_model);
  auto mesh = std::make_shared<Mesh>(gltf_model);
  _meshes[mesh_path] = std::make_tuple(mesh, texture);
  return { mesh, texture };
}

Cache::ModelPair
Cache::get_rect(const Core::Color& color)
{
  auto rect_id = "#rect";
  if (_meshes.contains(rect_id)) {
    return _meshes[rect_id];
  }
  std::vector<unsigned char> data { color.r, color.g, color.b, color.a };
  auto texture = std::make_shared<Texture>(1, 1, data);
  auto mesh = std::shared_ptr(create_quad());
  auto pair = std::make_tuple(mesh, texture);
  _meshes[rect_id] = pair;
  return pair;
}

Cache::SoundPtr
Cache::get_sound(const std::filesystem::path& sound_path)
{
  if (_sounds.contains(sound_path)) {
    return _sounds[sound_path];
  }
  std::shared_ptr<Sound::Sound> sound = load_sound(sound_path);
  _sounds[sound_path] = sound;
  return sound;
}
