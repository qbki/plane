#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "src/game_state/texture_type.h"
#include "src/mesh.h"
#include "src/texture.h"

const int MESH_IDX = 0;
const int TEXTURE_IDX = 1;

class Cache
{
public:
  using MeshPtr = std::shared_ptr<Mesh>;
  using TexturesPtr =
    std::shared_ptr<std::unordered_map<TextureType::Type, Texture>>;

private:
  std::unordered_map<std::string, std::tuple<MeshPtr, TexturesPtr>> _meshes;

public:
  Cache();

  std::tuple<MeshPtr, TexturesPtr> load(const std::string& mesh_file_name);
};
