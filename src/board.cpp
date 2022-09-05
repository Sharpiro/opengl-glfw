#include "board.h"
#include "temp.h"
#include <stdio.h>

Board board_new(int board_size, int circle_size) {
  auto board = Board{
    .size = board_size,
    .circle_size = circle_size,
    .circles = std::vector<Circle>(circle_size),
    .gl_draw_bounds = .5,
  };

  float simple_color = 1;
  for (auto i = 0; i < (int)board.circles.size(); i++) {
    simple_color -= 0.25;
    board.circles[i] = Circle{
      .id = i + 1,
      .index = i,
      .original_color = {0, simple_color, 0},
      .current_color = {0, simple_color, 0},
    };
  }
  return board;
}

static void resize_board_horizontal(Board *board, int new_size) {
  board->horizontal_lines = std::vector<Line>{};
  if (new_size < 1) {
    return;
  }
  auto section_size = 1.f / new_size;
  auto current = -board->gl_draw_bounds;
  for (int i = 0; i < new_size; i++) {
    current += section_size;
    auto line = Line{
      .start =
        {
          .position = {-board->gl_draw_bounds, current},
          .color = {0, 0, 0},
        },
      .end =
        {
          .position = {board->gl_draw_bounds, current},
          .color = {0, 0, 0},
        },
    };
    board->horizontal_lines.push_back(line);
  }

  auto line = Line{
    .start =
      {
        .position = {-board->gl_draw_bounds, -board->gl_draw_bounds},
        .color = {0, 0, 0},
      },
    .end =
      {
        .position = {board->gl_draw_bounds, -board->gl_draw_bounds},
        .color = {0, 0, 0},
      },
  };
  board->horizontal_lines.push_back(line);
}

static void resize_board_vertical(Board *board, int new_size) {
  board->vertical_lines = std::vector<Line>{};
  if (new_size < 1) {
    return;
  }
  auto section_size = 1.f / new_size;
  auto current = -board->gl_draw_bounds;
  for (int i = 0; i < new_size; i++) {
    current += section_size;
    auto line = Line{
      .start =
        {
          .position = {current, -board->gl_draw_bounds},
          .color = {0, 0, 0},
        },
      .end =
        {
          .position = {current, board->gl_draw_bounds},
          .color = {0, 0, 0},
        },
    };
    board->vertical_lines.push_back(line);
  }

  auto line = Line{
    .start =
      {
        .position = {-board->gl_draw_bounds, board->gl_draw_bounds},
        .color = {0, 0, 0},
      },
    .end =
      {
        .position = {-board->gl_draw_bounds, -board->gl_draw_bounds},
        .color = {0, 0, 0},
      },
  };
  board->horizontal_lines.push_back(line);
}

void resize_board(Board *board, int new_size) {
  board->size = new_size;
  board->squares = std::vector<Square>(board->size * board->size);
  resize_board_horizontal(board, new_size);
  resize_board_vertical(board, new_size);
}

struct BoardDimensions {
  float x_start;
  float x_end;
  float y_start;
  float y_end;
  float square_pixels_x;
  float square_pixels_y;
};

static BoardDimensions get_board_dimensions(Board *board, Point window) {
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
    square_pixels_y,
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

  auto click_index = square_y * board_size + square_x;
  return click_index;
}

BoardPressState board_on_press(Board *board, Point window, Point press) {
  auto board_dimensions = get_board_dimensions(board, window);
  auto press_index = get_square_index(board_dimensions, board->size, press);
  auto state = press_index >= 0
    ? BoardPressState{.in_bounds = true, .press_index = press_index}
    : BoardPressState{};
  return state;
}

BoardReleaseState board_on_release(Board *board, Point window, Click click) {
  auto board_dimensions = get_board_dimensions(board, window);
  auto press_index =
    get_square_index(board_dimensions, board->size, click.press);
  auto release_index =
    get_square_index(board_dimensions, board->size, click.release);
  auto valid_click = press_index != -1 && release_index != -1;
  auto board_state = BoardReleaseState{
    .valid_click = valid_click,
    .press_index = press_index,
    .release_index = release_index,
  };
  return board_state;
}

bool point_equals(Point a, Point b) {
  return a.x == b.x && a.y == b.y;
}

Circle *find_circle(Board *board, int id) {
  for (auto &circle : board->circles) {
    if (circle.id == id) return &circle;
  }
  panic("invalid circle id: %d\n", 99);
}

Square get_square(int board_size, int index) {
  auto y_index = index / board_size;
  auto x_index = index % board_size;
  auto square = Square{.x_index = x_index, .y_index = y_index};
  return square;
}
