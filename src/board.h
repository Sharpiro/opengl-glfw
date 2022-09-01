#pragma once

#include "vec.h"
#include <vector>

struct Triangle
{
  Vertex a;
  Vertex b;
  Vertex c;
};

struct Line
{
  Vertex start;
  Vertex end;
};

struct ChunkyCircle
{
  std::vector<Triangle> triangles;
};

struct Board
{
  int size;
  std::vector<Line> horizontal_lines;
  std::vector<Line> vertical_lines;
};

void resize_board(Board *board, int new_size);

void resize_board_vert(Board *board, int new_size);
