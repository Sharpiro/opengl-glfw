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

struct Circle {
  int circle_index;
  vec3 circle_color;
  // std::vector<Triangle> triangles;
};

struct Point {
  double x;
  double y;
};

struct Click {
  Point start;
  Point end;
};

enum SquareColor {
  White,
  Black
};

struct Square {
  int index;
  SquareColor color;
};

struct Board {
  int size;
  std::vector<Square> squares;
  int circle_index;
  vec3 circle_color;
  float gl_draw_bounds;
  std::vector<Line> horizontal_lines;
  std::vector<Line> vertical_lines;
};

void resize_board(Board* board, int new_size);

void board_on_press(Board* board, Point window, Point click);

void board_on_click(Board* board, Point window, Click click);

bool point_equals(Point pressed_cursor, Point cursor);
