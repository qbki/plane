#pragma once
#include "src/mesh.h"

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

std::unique_ptr<Mesh>
create_quad(float x = 1.0, float y = 1.0, float z = 0.0);

std::unique_ptr<Mesh>
create_ui_quad();
