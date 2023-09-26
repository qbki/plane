#include <algorithm>
#include <filesystem>
#include <glm/matrix.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "../utils.h"
#include "../camera.h"
#include "../material.h"
#include "../consts.h"
#include "model.h"
#include "graphic.h"


Model::Model(
  std::shared_ptr<Mesh> mesh,
  std::shared_ptr<Shader> shader,
  std::shared_ptr<Material> material,
  std::vector<std::shared_ptr<Texture>> textures
) : _mesh(mesh),
    _shader(shader),
    _material(material),
    _textures(textures)
{
  _basecolor_texture_idx = textures.at(0)->type();
}


void Model::use_basecolor_texture(unsigned int type) {
  auto begin_it = begin(this->_textures);
  auto end_it = end(this->_textures);
  auto found_it = std::find_if(
      begin_it,
      end_it,
      [&type](std::shared_ptr<Texture>v) { return v->type() == type; }
  );
  if (found_it == end_it) {
    std::cout << "Basecolor '" << type << "' was not found" << '\n';
  }
  this->_basecolor_texture_idx = found_it == end_it
    ? 0
    : std::distance(begin_it, found_it);
}


void Model::draw(const Camera& camera, const SunLight& light, float elapsed_seconds) const {
  if (this->is_active()) {
    _shader->use();
    _shader->uniform("u_PV", camera.pv());
    _shader->uniform("u_elapsed_seconds", elapsed_seconds);
    _shader->uniform("u_M", _transform);
    _shader->uniform("u_normal_matrix", glm::transpose(glm::inverse(glm::mat3(_transform))));
    _shader->uniform("u_camera_pos", camera.position());
    _shader->uniform("u_light.color", light.color());
    _shader->uniform("u_light.direction", light.direction());
    _shader->uniform("u_material.ambient", _material->ambient());
    _shader->uniform("u_material.specular", _material->specular());
    _shader->uniform("u_material.shininess", _material->shininess());
    _shader->uniform("gamma_in", GAMMA);
    _shader->uniform("gamma_out", GAMMA);

    auto texture = _textures[this->_basecolor_texture_idx];
    texture->use(0);
    _shader->uniform("main_texture", 0);

    _mesh->draw();

    Graphic::draw(camera, light, elapsed_seconds);
  }
}
