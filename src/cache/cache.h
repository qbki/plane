#pragma once
#include <filesystem>
#include <string>
#include <tuple>
#include <unordered_map>

#include "src/mesh.h"
#include "src/sound/sound.h"
#include "src/texture.h"

const int MESH_IDX = 0;
const int TEXTURE_IDX = 1;

class Cache
{
public:
  using MeshPtr = std::shared_ptr<Mesh>;
  using TexturePtr = std::shared_ptr<Texture>;
  using SoundPtr = std::shared_ptr<Sound::Sound>;

private:
  std::unordered_map<std::string, std::tuple<MeshPtr, TexturePtr>> _meshes;
  std::unordered_map<std::string, SoundPtr> _sounds;

public:
  Cache();

  std::tuple<MeshPtr, TexturePtr> get_model(
    const std::filesystem::path& mesh_file_name);
  SoundPtr get_sound(const std::filesystem::path& sound_file_name);
};
