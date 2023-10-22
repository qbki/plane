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
  std::vector<std::shared_ptr<Texture>> textures
) : _mesh(mesh),
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
    this->_basecolor_texture_idx = 0;
  } else {
    this->_basecolor_texture_idx = std::distance(begin_it, found_it);
  }
}


std::shared_ptr<Texture> Model::texture() {
  return _textures[this->_basecolor_texture_idx];
}


void Model::draw() {
  if (this->is_active()) {
    _mesh->draw();
  }
}
