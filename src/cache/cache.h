#pragma once
#include <filesystem>
#include <string>
#include <tuple>
#include <unordered_map>

#include "src/mesh.h"
#include "src/sound/sound.h"
#include "src/texture.h"
#include "src/utils/color.h"

class Cache
{
public:
  using MeshPtr = std::shared_ptr<Mesh>;
  using TexturePtr = std::shared_ptr<Texture>;
  using ModelPair = std::tuple<MeshPtr, TexturePtr>;
  using SoundPtr = std::shared_ptr<Sound::Sound>;

private:
  std::unordered_map<std::string, std::tuple<MeshPtr, TexturePtr>> _meshes;
  std::unordered_map<std::string, SoundPtr> _sounds;

public:
  Cache();

  ModelPair get_model(const std::filesystem::path& mesh_path);
  ModelPair get_rect(const Core::Color& color);
  SoundPtr get_sound(const std::filesystem::path& sound_path);
};
