#include "rev_game.h"
#include <stdio.h>

GameSate rev_init() {
  return GameSate{.name = "Horz Game"};
}

void rev_on_mouse_press(Board *board, BoardPressState state) {
  for (auto &circle : board->circles) {
    if (circle.index == state.press_index) {
      vec3 dark_green = {0.f, .6f, .0f};
      memcpy(&circle.current_color, &dark_green, sizeof(vec3));
      break;
    }
  }
}

void rev_on_mouse_release(Board *board, BoardReleaseState state) {
  for (auto &circle : board->circles) {
    memcpy(&circle.current_color, &circle.original_color, sizeof(vec3));
    if (state.valid_click && state.press_index == circle.index) {
      auto valid_move = true;
      if (valid_move) {
        circle.index = state.release_index;
      }
      break;
    }
  }
}

void rev_update(Board *board, GameSate *game_state) {
  if (game_state->game_over) return;

  auto start = board->size - 1;
  for (auto i = start; i < board->size * board->size; i += board->size) {
    for (auto circle : board->circles) {
      if (circle.index == i) {
        game_state->game_over = true;
      }
    }
  }
}
