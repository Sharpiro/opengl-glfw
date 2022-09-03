#include "board.h"
#include <stdio.h>

static void resize_board_horizontal(Board* board, int new_size) {
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
        .start = {
          .position = {-board->gl_draw_bounds, current},
          .color = {0, 0, 0}
        },
        .end = {
          .position = {board->gl_draw_bounds, current},
          .color = {0, 0, 0}
        }
    };
    board->horizontal_lines.push_back(line);
  }

  auto line = Line{
      .start = {
        .position = {-board->gl_draw_bounds, -board->gl_draw_bounds},
        .color = {0, 0, 0}
      },
      .end = {
        .position = {board->gl_draw_bounds, -board->gl_draw_bounds},
        .color = {0, 0, 0}
      }
  };
  board->horizontal_lines.push_back(line);
}

static void resize_board_vertical(Board* board, int new_size) {
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
        .start = {
          .position = {current, -board->gl_draw_bounds},
          .color = {0, 0, 0}
        },
        .end = {
          .position = {current, board->gl_draw_bounds},
          .color = {0, 0, 0}
        }
    };
    board->vertical_lines.push_back(line);
  }

  auto line = Line{
      .start = {
        .position = {-board->gl_draw_bounds, board->gl_draw_bounds},
        .color = {0, 0, 0}
      },
      .end = {
        .position = {-board->gl_draw_bounds, -board->gl_draw_bounds},
        .color = {0, 0, 0}
      }
  };
  board->horizontal_lines.push_back(line);
}

void do_nothing(Square(*temp)[4]) {

}

void do_nothing_else(Square* temp[4]) {

}

void resize_board(Board* board, int new_size) {
  board->circle_index = 0;
  board->size = new_size;
  board->squares = std::vector<Square>(board->size * board->size);
  for (auto& square : board->squares) {
    //
  }
  resize_board_horizontal(board, new_size);
  resize_board_vertical(board, new_size);
}

struct BoardDimensions
{
  float x_start;
  float x_end;
  float y_start;
  float y_end;
  float square_pixels_x;
  float square_pixels_y;
};

static BoardDimensions get_board_dimensions(
  Board* board,
  Point window
) {
  auto x_pixels = (float)window.x * board->gl_draw_bounds;
  auto y_pixels = (float)window.y * board->gl_draw_bounds;
  auto x_start = x_pixels / 2;
  auto x_end = x_start + x_pixels;
  auto y_start = y_pixels / 2;
  auto y_end = y_start + y_pixels;
  auto square_pixels_x = floor(x_pixels / board->size);
  auto square_pixels_y = floor(y_pixels / board->size);

  auto board_dimensions = BoardDimensions{
    x_start,
    x_end,
    y_start,
    y_end,
    square_pixels_x,
    square_pixels_y
  };
  return board_dimensions;
}

static int get_square_index(
  BoardDimensions dimensions,
  int board_size,
  Point click
) {
  auto x_start = dimensions.x_start;
  auto y_start = dimensions.y_start;
  auto square_pixels_x = dimensions.square_pixels_x;
  auto square_pixels_y = dimensions.square_pixels_y;

  if (click.x < x_start || click.x > dimensions.x_end) return -1;
  if (click.y < y_start || click.y > dimensions.y_end) return -1;

  auto square_x = (int)floor((click.x - x_start) / square_pixels_x);
  auto square_y = (int)floor((click.y - y_start) / square_pixels_y);
  printf("window location (%f, %f)\n", click.x, click.y);
  printf("board  location (%d, %d)\n", square_x, square_y);

  auto click_index = square_y * board_size + square_x;
  return click_index;
}

void board_on_press(Board* board, Point window, Point click) {
  auto board_dimensions = get_board_dimensions(board, window);
  auto press_index = get_square_index(
    board_dimensions,
    board->size,
    click
  );
  if (board->circle_index == press_index) {
    vec3 dark_green = { 0.f, .6f, .0f };
    memcpy(&board->circle_color, &dark_green, sizeof(vec3));
  }
}

void board_on_click(Board* board, Point window, Click click) {
  vec3 green = { 0.f, 1.f, 0.f };
  memcpy(&board->circle_color, &green, sizeof(vec3));
  auto board_dimensions = get_board_dimensions(board, window);
  auto click_start_index = get_square_index(
    board_dimensions,
    board->size,
    click.start
  );
  auto click_end_index = get_square_index(
    board_dimensions,
    board->size,
    click.end
  );
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

bool point_equals(Point a, Point b) {
  return a.x == b.x && a.y == b.y;
}