#include "vert_game.h"
#include <stdio.h>

void common() {
  puts("running vert game");
}

void vert_on_mouse_press() {
  //
}

static bool is_valid_move(int board_size, Circle *source, Circle *destination) {
  if (source->index == destination->index) return false;

  if (source->index + board_size == destination->index) return true;
  if (source->index + 1 == destination->index) return true;
  return false;
}

void vert_on_mouse_release(Board *board, BoardState board_state) {
  for (auto &changed_circle : board_state.changed_circles) {
    auto board_circle = find_circle(board, changed_circle.id);
    // if (changed_circle.index != board_circle->index) {
    if (is_valid_move(board->size, board_circle, &changed_circle)) {
      board_circle->index = changed_circle.index;
    }
  }
}

void vert_update(Board *board, GameSate *game_state) {
  if (game_state->game_over) return;

  auto start = board->size * (board->size - 1);
  for (auto i = start; i < board->size * board->size; i++) {
    for (auto circle : board->circles) {
      if (circle.index == i) {
        game_state->game_over = true;
      }
    }
  }
}
