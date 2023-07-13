#include <algorithm>
#include <filesystem>
#include <glm/matrix.hpp>
#include <memory>

#include "./model.h"
#include "./utils.h"
#include "./camera.h"


Model::Model(
  std::shared_ptr<Mesh> _mesh,
  std::shared_ptr<Shader> _shader
) : mesh(_mesh),
    shader(_shader) {}


void Model::draw(const Camera& camera, const SunLight& light) {
  shader->use();
  shader->setUniform("u_PVM", camera.pv() * this->transform);
  shader->setUniform("u_normal_matrix", glm::transpose(glm::inverse(glm::mat3(this->transform))));
  shader->setUniform("u_camera_pos", camera.get_position());
  shader->setUniform("u_light.color", light.get_color());
  shader->setUniform("u_light.direction", light.get_direction());
  mesh->draw();
}


std::unique_ptr<Model> Model::load(
  const std::string& mesh_file_name,
  const std::string& vertex_shader_file_name,
  const std::string& fragment_shader_file_name
) {
  auto vertex_shader = load_text(vertex_shader_file_name);
  auto fragment_shader = load_text(fragment_shader_file_name);

  std::shared_ptr<Shader> shader(new Shader);
  shader->load(vertex_shader, fragment_shader);

  auto raw_mesh = load_mesh(mesh_file_name);
  std::shared_ptr<Mesh> mesh(new Mesh(raw_mesh));

  return std::unique_ptr<Model>(new Model(mesh, shader));
}
