#pragma once
#include <memory>

import pln.meshes;

struct RectSize
{
  int width = 0;
  int height = 0;

  RectSize() = default;
  RectSize(int _width, int _height);
};

template<typename T>
struct Rect
{
  T x;
  T y;
  T width;
  T height;
};

std::unique_ptr<pln::meshes::Mesh>
create_quad(float x = 1.0, float y = 1.0, float z = 0.0);

std::unique_ptr<pln::meshes::Mesh>
create_ui_quad();
