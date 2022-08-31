#include "vec.h"
#include <vector>

struct Line
{
  Vertex start;
  Vertex end;
};

struct Board
{
  std::vector<Line> horizontal_lines;
  std::vector<Line> vertical_lines;
};

void resize_board(Board *board, int new_size);

void resize_board_vert(Board *board, int new_size);
