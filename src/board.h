#pragma once

#include "vec.h"
#include <vector>

struct Triangle {
  Vertex a;
  Vertex b;
  Vertex c;
};

struct Line {
  Vertex start;
  Vertex end;
};

struct Circle {
  int id;
  int index;
  vec3 original_color;
  vec3 current_color;
  // std::vector<Triangle> triangles;
};

struct Point {
  double x;
  double y;
};

struct Click {
  Point press;
  Point release;
};

enum SquareColor {
  White,
  Black,
};

struct Square {
  int x_index;
  int y_index;
  // int index;
  // SquareColor color;
};

struct Board {
  int size;
  std::vector<Square> squares;
  // int circle_index;
  // vec3 circle_color;
  int circle_size;
  std::vector<Circle> circles;
  float gl_draw_bounds;
  std::vector<Line> horizontal_lines;
  std::vector<Line> vertical_lines;
};

struct BoardPressState {
  bool in_bounds;
  int press_index = -1;
};

struct BoardReleaseState {
  bool valid_click;
  int press_index = -1;
  int release_index = -1;
};

Board board_new(int board_size, int circle_size);

void resize_board(Board *board, int new_size);

BoardPressState board_on_press(Board *board, Point window, Point click);

BoardReleaseState board_on_release(Board *board, Point window, Click click);

Circle *find_circle(Board *board, int id);

Square get_square(int board_size, int index);
