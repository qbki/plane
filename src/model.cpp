#include <algorithm>
#include <filesystem>
#include <glm/matrix.hpp>
#include <iostream>
#include <memory>

#include "./model.h"
#include "./utils.h"
#include "./camera.h"
#include "./graphic.h"
#include "./material.h"


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
