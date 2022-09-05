#pragma once

#include "../board.h"

struct GameSate {
  bool game_over;
  bool dumb_bool;
};

void common();

void vert_on_mouse_press(Board *board, BoardPressState state);

void vert_on_mouse_release(Board *board, BoardReleaseState board_state);

void vert_update(Board *board, GameSate *game_state);
