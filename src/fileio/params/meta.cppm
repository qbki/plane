module;
#include <glm/vec3.hpp>

export module pln.fileio.params.meta;

namespace pln::fileio::params {

export
struct CameraParams
{
  glm::vec3 position { 0, 0, 0 };
};


export
struct BoundaryParams
{
  glm::vec3 min { 0, 0, 0 };
  glm::vec3 max { 0, 0, 0 };
};


export
struct LevelMetaParams
{
  CameraParams camera;
  BoundaryParams boundaries;
};

}
