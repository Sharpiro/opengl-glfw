#include "board.h"

void resize_board(Board *board, int new_size)
{
  board->horizontal_lines = std::vector<Line>{};
  // auto section_size = 2.f / new_size;
  if (new_size < 1)
  {
    return;
  }
  auto section_size = 1.f / new_size;
  auto top = -0.5f;
  auto current_y = top;
  for (int i = 0; i < new_size; i++)
  {
    current_y += section_size;
    auto line = Line{
        .start = {.position = {-.5, current_y}, .color = {0, 1, 0}},
        .end = {.position = {.5, current_y}, .color = {1, 0, 0}}};
    board->horizontal_lines.push_back(line);
  }
  auto line = Line{
      .start = {.position = {-.5, -.5}, .color = {0, 1, 0}},
      .end = {.position = {.5, -.5f}, .color = {1, 0, 0}}};
  board->horizontal_lines.push_back(line);
}

void resize_board_vert(Board *board, int new_size)
{
  board->vertical_lines = std::vector<Line>{};
  if (new_size < 1)
  {
    return;
  }
  auto section_size = 1.f / new_size;
  auto left = -0.5f;
  auto current_x = left;
  for (int i = 0; i < new_size; i++)
  {
    current_x += section_size;
    auto line = Line{
        .start = {.position = {current_x, -.5}, .color = {0, 1, 0}},
        .end = {.position = {current_x, .5}, .color = {1, 0, 0}}};
    board->vertical_lines.push_back(line);
  }
  auto line = Line{
      .start = {.position = {-.5, .5}, .color = {0, 1, 0}},
      .end = {.position = {-.5, -.5f}, .color = {1, 0, 0}}};
  board->horizontal_lines.push_back(line);
}
