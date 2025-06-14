module;
#include <glm/vec3.hpp>

export module pln.materals;

namespace pln::materials {

export class Material
{
private:
  glm::vec3 _ambient;
  glm::vec3 _specular;
  float _shininess;

public:
  constexpr static float DEFAULT_AMBIENT_FRACTION {0.03f};
  constexpr static glm::vec3 DEFAULT_SPECULAR {glm::vec3(0.5, 0.5, 0.5)};
  constexpr static float DEFAULT_SHININESS {30.f};


  explicit Material(glm::vec3 color)
    : _ambient(color * Material::DEFAULT_AMBIENT_FRACTION)
    , _specular(Material::DEFAULT_SPECULAR)
    , _shininess(Material::DEFAULT_SHININESS)
  {
  }


  explicit Material(glm::vec3 ambient, glm::vec3 specular, float shininess)
    : _ambient(ambient)
    , _specular(specular)
    , _shininess(shininess)
  {
  }


  [[nodiscard]] glm::vec3 ambient() const
  {
    return this->_ambient;
  }


  void ambient(const glm::vec3& value)
  {
    this->_ambient = value;
  }


  [[nodiscard]] glm::vec3 specular() const
  {
    return this->_specular;
  }


  void specular(const glm::vec3& value)
  {
    this->_specular = value;
  }


  [[nodiscard]] float shininess() const
  {
    return this->_shininess;
  }


  void shininess(float value)
  {
    this->_shininess = value;
  }
};

}
