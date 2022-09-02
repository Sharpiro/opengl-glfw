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

struct Circle
{
  std::vector<Triangle> triangles;
};

struct Point
{
  double x;
  double y;
};

struct Click
{
  Point start;
  Point end;
};

struct Board
{
  // int squares[4][4];
  // int squares[16];
  int size;
  int circle_index;
  float gl_draw_bounds;
  std::vector<Line> horizontal_lines;
  std::vector<Line> vertical_lines;
};

void resize_board(Board *board, int new_size);

void resize_board_vert(Board *board, int new_size);

void board_handle_click(Board *board, Point window, Click click);

bool point_equals(Point pressed_cursor, Point cursor);
