#include "projectile.h"

Projectile::Projectile(std::shared_ptr<Model> model)
  : _model(model) {}


void Projectile::model(std::shared_ptr<Model> value) {
  this->_model = value;
}


std::shared_ptr<Model> Projectile::model() const {
  return this->_model;
}


void Projectile::start_point(const glm::vec3& value) {
  this->_start_point = value;
}


glm::vec3 Projectile::start_point() const {
  return this->_start_point;
}


void Projectile::distance(double value) {
  this->_distance = value;
}


double Projectile::distance() const {
  return this->_distance;
}
