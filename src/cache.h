#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "material.h"
#include "mesh.h"
#include "models.h"
#include "shader.h"
#include "texture.h"

const int MESH_IDX = 0;
const int MATERIAL_IDX = 1;
const int TEXTURE_IDX = 2;

class Cache {
private:
  using mesh_ptr = std::shared_ptr<Mesh>;
  using material_ptr = std::shared_ptr<Material>;
  using texture_ptr = std::shared_ptr<Texture>;

  std::unordered_map<std::string, std::tuple<mesh_ptr, material_ptr, texture_ptr>> meshes;
  std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
public:
  Cache();

  std::shared_ptr<Model> load(
    const std::string& mesh_file_name,
    const std::string& vertex_shader_file_name,
    const std::string& fragment_shader_file_name);
};
