#include "board.h"

void resize_board(Board *board, int new_size)
{
  board->horizontal_lines = std::vector<Line>{};
  if (new_size < 1)
  {
    return;
  }
  auto section_size = 1.f / new_size;
  auto current = -board->draw_bounds;
  for (int i = 0; i < new_size; i++)
  {
    current += section_size;
    auto line = Line{
        .start = {.position = {-board->draw_bounds, current}, .color = {0, 1, 0}},
        .end = {.position = {board->draw_bounds, current}, .color = {1, 0, 0}}};
    board->horizontal_lines.push_back(line);
  }
  auto line = Line{
      .start = {.position = {-board->draw_bounds, -board->draw_bounds}, .color = {0, 1, 0}},
      .end = {.position = {board->draw_bounds, -board->draw_bounds}, .color = {1, 0, 0}}};
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
  auto current = -board->draw_bounds;
  for (int i = 0; i < new_size; i++)
  {
    current += section_size;
    auto line = Line{
        .start = {.position = {current, -board->draw_bounds}, .color = {0, 1, 0}},
        .end = {.position = {current, board->draw_bounds}, .color = {1, 0, 0}}};
    board->vertical_lines.push_back(line);
  }
  auto line = Line{
      .start = {.position = {-board->draw_bounds, board->draw_bounds}, .color = {0, 1, 0}},
      .end = {.position = {-board->draw_bounds, -board->draw_bounds}, .color = {1, 0, 0}}};
  board->horizontal_lines.push_back(line);
}
