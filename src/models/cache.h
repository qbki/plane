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


class Cache {
public:
  using MeshPtr = std::shared_ptr<Mesh>;
  using MaterialPtr = std::shared_ptr<Material>;
  using Textures = std::vector<std::shared_ptr<Texture>>;

private:
  std::unordered_map<std::string, std::tuple<MeshPtr, MaterialPtr, Textures>> _meshes;

public:
  Cache();

  std::tuple<std::shared_ptr<Mesh>, std::shared_ptr<Texture>>
  load(const std::string& mesh_file_name);
};
