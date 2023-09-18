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
  std::shared_ptr<Mesh> mesh,
  std::shared_ptr<Shader> shader,
  std::shared_ptr<Material> material
) : mesh(mesh),
    shader(shader),
    material(material) {}


void Model::draw(const Camera& camera, const SunLight& light, float elapsed_seconds) const {
  if (this->is_active()) {
    shader->use();
    shader->uniform("u_PV", camera.pv());
    shader->uniform("u_elapsed_seconds", elapsed_seconds);
    shader->uniform("u_M", this->_transform);
    shader->uniform("u_normal_matrix", glm::transpose(glm::inverse(glm::mat3(this->_transform))));
    shader->uniform("u_camera_pos", camera.position());
    shader->uniform("u_light.color", light.color());
    shader->uniform("u_light.direction", light.direction());
    shader->uniform("u_material.ambient", this->material->ambient());
    shader->uniform("u_material.diffuse", this->material->diffuse());
    shader->uniform("u_material.specular", this->material->specular());
    shader->uniform("u_material.shininess", this->material->shininess());
    mesh->draw();

    Graphic::draw(camera, light, elapsed_seconds);
  }
}
