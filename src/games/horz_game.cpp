#include "horz_game.h"
#include <stdio.h>

void common() {
  puts("running horz game");
}

void horz_on_mouse_press() {
  //
}

void horz_on_mouse_release(Board *board, BoardReleaseState board_state) {
  for (auto &changed_circle : board_state.changed_circles) {
    auto board_circle = find_circle(board, changed_circle.id);
    if (changed_circle.index != board_circle->index) {
      board_circle->index = changed_circle.index;
    }
  }
}

void horz_update(Board *board, GameSate *game_state) {
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
