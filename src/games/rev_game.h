#pragma once

#include "../board.h"

struct GameSate {
  bool game_over;
  bool dumb_bool;
  const char *name;
};

GameSate rev_init();

void rev_on_mouse_press(Board *board, BoardPressState state);

void rev_on_mouse_release(Board *board, BoardReleaseState board_state);

void rev_update(Board *board, GameSate *game_state);
