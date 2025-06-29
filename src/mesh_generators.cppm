module;
#include <memory>

export module pln.mesh_generators;

import pln.mesh;

namespace pln::mesh {

export
std::unique_ptr<pln::mesh::Mesh>
create_quad(float x = 1.0f, float y = 1.0f, float z = 0.0f)
{
  std::vector<float> vertices = {
    -x, y,  z, //
    x,  y,  z, //
    -x, -y, z, //
    x,  -y, z, //
  };
  std::vector<float> normals = {
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0, //
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0, //
  };
  std::vector<float> tex_coords = {
    0.0, 1.0, 1.0, 1.0, //
    0.0, 0.0, 1.0, 0.0, //
  };
  std::vector<unsigned int> indices = {
    2, 1, 0, //
    2, 3, 1, //
  };
  return std::make_unique<pln::mesh::Mesh>(vertices, normals, tex_coords, indices);
}


export
std::unique_ptr<pln::mesh::Mesh>
create_y_reflected_quad()
{
  std::vector<float> vertices = {
    -1, 1,  0, //
    1,  1,  0, //
    -1, -1, 0, //
    1,  -1, 0, //
  };
  std::vector<float> normals = {
    0.0, 0.0, 1.0, //
    0.0, 0.0, 1.0, //
    0.0, 0.0, 1.0, //
    0.0, 0.0, 1.0, //
  };
  std::vector<float> tex_coords = {
    0.0, 0.0, //
    1.0, 0.0, //
    0.0, 1.0, //
    1.0, 1.0, //
  };
  std::vector<unsigned int> indices = {
    2, 1, 0, //
    2, 3, 1, //
  };
  return std::make_unique<pln::mesh::Mesh>(vertices, normals, tex_coords, indices);
}


export
std::unique_ptr<pln::mesh::Mesh>
create_ui_quad()
{
  std::vector<float> vertices = {
    0, 0, 0, //
    1, 0, 0, //
    0, 1, 0, //
    1, 1, 0, //
  };
  std::vector<float> normals = {
    0.0, 0.0, 1.0, //
    0.0, 0.0, 1.0, //
    0.0, 0.0, 1.0, //
    0.0, 0.0, 1.0, //
  };
  std::vector<float> tex_coords = {
    0.0, 0.0, //
    1.0, 0.0, //
    0.0, 1.0, //
    1.0, 1.0, //
  };
  std::vector<unsigned int> indices = {
    2, 1, 0, //
    2, 3, 1, //
  };
  return std::make_unique<pln::mesh::Mesh>(vertices, normals, tex_coords, indices);
}

}
