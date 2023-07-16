#include <algorithm>
#include <filesystem>
#include <glm/matrix.hpp>
#include <iostream>
#include <memory>

#include "./model.h"
#include "./utils.h"
#include "./camera.h"
#include "graphic.h"
#include "material.h"


glm::vec3 exctract_material_color(tinygltf::Model& model) {
  auto material_id = model.meshes.at(0).primitives.at(0).material;
  auto material = model.materials.at(material_id);
  auto color = material.pbrMetallicRoughness.baseColorFactor;
  return glm::vec3(color.at(0), color.at(1), color.at(2));
}


Model::Model(
  std::shared_ptr<Mesh> _mesh,
  std::shared_ptr<Shader> _shader,
  std::shared_ptr<Material> _material
) : mesh(_mesh),
    shader(_shader),
    material(_material) {}


void Model::draw(const Camera& camera, const SunLight& light) const {
  shader->use();
  shader->setUniform("u_PVM", camera.pv() * this->transform);
  shader->setUniform("u_normal_matrix", glm::transpose(glm::inverse(glm::mat3(this->transform))));
  shader->setUniform("u_camera_pos", camera.get_position());
  shader->setUniform("u_light.color", light.get_color());
  shader->setUniform("u_light.direction", light.get_direction());
  shader->setUniform("u_material.ambient", this->material->get_ambient());
  shader->setUniform("u_material.diffuse", this->material->get_diffuse());
  shader->setUniform("u_material.specular", this->material->get_specular());
  shader->setUniform("u_material.shininess", this->material->get_shininess());
  mesh->draw();

  Graphic::draw(camera, light);
}


std::shared_ptr<Model> Model::load(
  const std::string& mesh_file_name,
  const std::string& vertex_shader_file_name,
  const std::string& fragment_shader_file_name
) {
  auto vertex_shader = load_text(vertex_shader_file_name);
  auto fragment_shader = load_text(fragment_shader_file_name);

  std::shared_ptr<Shader> shader(new Shader);
  shader->load(vertex_shader, fragment_shader);

  auto gltf_model = load_gltf_model(mesh_file_name);
  std::shared_ptr<Mesh> mesh(new Mesh(gltf_model));

  auto color = exctract_material_color(gltf_model);
  std::shared_ptr<Material> material(new Material(color));

  return std::shared_ptr<Model>(new Model(mesh, shader, material));
}
