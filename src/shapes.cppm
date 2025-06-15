module;
#include <memory>

export module pln.shapes;

import pln.meshes;

namespace pln::shapes {

export
struct RectSize
{
  int width = 0;
  int height = 0;

  RectSize() = default;

  RectSize(int _width, int _height)
    : width(_width)
    , height(_height)
  {
  }
};

export
template<typename T>
struct Rect
{
  T x;
  T y;
  T width;
  T height;
};


export
std::unique_ptr<pln::meshes::Mesh>
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
  return std::make_unique<pln::meshes::Mesh>(vertices, normals, tex_coords, indices);
}


export
std::unique_ptr<pln::meshes::Mesh>
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
  return std::make_unique<pln::meshes::Mesh>(vertices, normals, tex_coords, indices);
}


export
std::unique_ptr<pln::meshes::Mesh>
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
  return std::make_unique<pln::meshes::Mesh>(vertices, normals, tex_coords, indices);
}

}
