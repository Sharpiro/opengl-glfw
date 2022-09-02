#include "board.h"
#include <stdio.h>

void resize_board(Board *board, int new_size)
{
  board->horizontal_lines = std::vector<Line>{};
  if (new_size < 1)
  {
    return;
  }
  auto section_size = 1.f / new_size;
  auto current = -board->gl_draw_bounds;
  for (int i = 0; i < new_size; i++)
  {
    current += section_size;
    auto line = Line{
        .start = {.position = {-board->gl_draw_bounds, current}, .color = {0, 1, 0}},
        .end = {.position = {board->gl_draw_bounds, current}, .color = {1, 0, 0}}};
    board->horizontal_lines.push_back(line);
  }
  auto line = Line{
      .start = {.position = {-board->gl_draw_bounds, -board->gl_draw_bounds}, .color = {0, 1, 0}},
      .end = {.position = {board->gl_draw_bounds, -board->gl_draw_bounds}, .color = {1, 0, 0}}};
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
  auto current = -board->gl_draw_bounds;
  for (int i = 0; i < new_size; i++)
  {
    current += section_size;
    auto line = Line{
        .start = {.position = {current, -board->gl_draw_bounds}, .color = {0, 1, 0}},
        .end = {.position = {current, board->gl_draw_bounds}, .color = {1, 0, 0}}};
    board->vertical_lines.push_back(line);
  }
  auto line = Line{
      .start = {.position = {-board->gl_draw_bounds, board->gl_draw_bounds}, .color = {0, 1, 0}},
      .end = {.position = {-board->gl_draw_bounds, -board->gl_draw_bounds}, .color = {1, 0, 0}}};
  board->horizontal_lines.push_back(line);
}

static int get_square_index(int board_size, int x_start, int x_end,
                            int y_start, int y_end, int board_pixels_x,
                            int board_pixels_y, Point click)
{
  if (click.x < x_start || click.x > x_end)
    return -1;
  if (click.y < y_start || click.y > y_end)
    return -1;

  auto square_x = (int)floor((click.x - x_start) / board_pixels_x);
  auto square_y = (int)floor((click.y - y_start) / board_pixels_y);
  printf("window location (%f, %f)\n", click.x, click.y);
  printf("board  location (%d, %d)\n", square_x, square_y);

  auto click_index = square_y * board_size + square_x;
  return click_index;
}

void board_handle_click(Board *board, Point window, Click click)
{
  auto x_pixels = (float)window.x * board->gl_draw_bounds;
  auto y_pixels = (float)window.y * board->gl_draw_bounds;
  auto x_start = x_pixels / 2;
  auto x_end = x_start + x_pixels;
  auto y_start = y_pixels / 2;
  auto y_end = y_start + y_pixels;
  auto square_pixels_x = floor(x_pixels / board->size);
  auto square_pixels_y = floor(y_pixels / board->size);

  auto click_start_index = get_square_index(board->size, x_start, x_end, y_start, y_end,
                                            square_pixels_x, square_pixels_y,
                                            click.start);
  auto click_end_index = get_square_index(board->size, x_start, x_end, y_start, y_end,
                                          square_pixels_x, square_pixels_y,
                                          click.end);
  if (click_start_index == -1 || click_end_index == -1)
  {
    puts("outside board");
    return;
  }
  if (click_start_index == board->circle_index)
  {
    board->circle_index = click_end_index;
  }
  auto has_circle = click_end_index == board->circle_index;
  printf("has circle: %s\n", has_circle ? "true" : "false");
}

// convert pixels to opengl coords
// auto x = 2.f / window.x;
// auto y = 2.f / window.y;
// auto xx = x * click.x - 1;
// auto yy = (y * click.y - 1) * -1;

bool point_equals(Point a, Point b)
{
  return a.x == b.x && a.y == b.y;
}