#include "shapes.h"

RectSize::RectSize(int _width, int _height)
  : width(_width)
  , height(_height)
{
}

std::unique_ptr<Mesh>
create_quad(float x, float y, float z)
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
  return std::make_unique<Mesh>(vertices, normals, tex_coords, indices);
}

std::unique_ptr<Mesh>
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
  return std::make_unique<Mesh>(vertices, normals, tex_coords, indices);
}

std::unique_ptr<Mesh>
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
  return std::make_unique<Mesh>(vertices, normals, tex_coords, indices);
}
