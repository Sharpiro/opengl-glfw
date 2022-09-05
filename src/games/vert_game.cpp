#include "vert_game.h"
#include <stdio.h>

void common() {
}

void vert_on_mouse_press(Board *board, BoardPressState state) {
  for (auto &circle : board->circles) {
    if (circle.index == state.press_index) {
      vec3 dark_green = {0.f, .6f, .0f};
      memcpy(&circle.current_color, &dark_green, sizeof(vec3));
      break;
    }
  }
}

static bool is_valid_move(int board_size, int source_index, int dest_index) {
  if (source_index == dest_index) return false;

  auto source_square = get_square(board_size, source_index);
  auto dest_square = get_square(board_size, dest_index);

  // x compare
  if (source_square.y_index == dest_square.y_index) {
    if (source_square.x_index + 1 == dest_square.x_index) {
      return true;
    }
    if (source_square.x_index - 1 == dest_square.x_index) {
      return true;
    }
  }

  // y compare
  if (source_square.x_index == dest_square.x_index) {
    if (source_square.y_index + 1 == dest_square.y_index) {
      return true;
    }
    if (source_square.y_index - 1 == dest_square.y_index) {
      return true;
    }
  }

  return false;
}

void vert_on_mouse_release(Board *board, BoardReleaseState state) {
  for (auto &circle : board->circles) {
    memcpy(&circle.current_color, &circle.original_color, sizeof(vec3));
    if (state.valid_click && state.press_index == circle.index) {
      auto valid_move =
        is_valid_move(board->size, circle.index, state.release_index);
      if (valid_move) {
        circle.index = state.release_index;
      }
      break;
    }
  }
}

void vert_update(Board *board, GameSate *game_state) {
  if (game_state->game_over) return;

  auto start = board->size * (board->size - 1);
  for (auto i = start; i < board->size * board->size; i++) {
    for (auto circle : board->circles) {
      if (circle.index == i) {
        // game_state->game_over = true;
      }
    }
  }
}
