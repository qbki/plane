#pragma once
#include "../models/index.h"

class Projectile {
private:
  std::shared_ptr<Model> _model;
  glm::vec3 _start_point;
  double _distance;
public:
  Projectile(std::shared_ptr<Model>);

  void model(std::shared_ptr<Model> value);
  std::shared_ptr<Model> model() const;

  void start_point(const glm::vec3& value);
  glm::vec3 start_point() const;

  void distance(double value);
  double distance() const;
};
