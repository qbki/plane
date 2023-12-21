#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../material.h"
#include "../mesh.h"
#include "../shader.h"
#include "../texture.h"

const int MESH_IDX = 0;
const int MATERIAL_IDX = 1;
const int TEXTURE_IDX = 2;

class Cache
{
public:
  using MeshPtr = std::shared_ptr<Mesh>;
  using MaterialPtr = std::shared_ptr<Material>;
  using TexturesPtr = std::shared_ptr<std::vector<Texture>>;

private:
  std::unordered_map<std::string, std::tuple<MeshPtr, MaterialPtr, TexturesPtr>>
    _meshes;

public:
  Cache();

  std::tuple<MeshPtr, TexturesPtr> load(const std::string& mesh_file_name);
};
