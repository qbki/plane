#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "./material.h"
#include "./shader.h"
#include "./mesh.h"
#include "./model.h"

class Cache {
private:
  std::unordered_map<std::string, std::shared_ptr<Material>> materials;
  std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
  std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
public:
  Cache();

  std::shared_ptr<Model> load(
    const std::string& mesh_file_name,
    const std::string& vertex_shader_file_name,
    const std::string& fragment_shader_file_name);
};
