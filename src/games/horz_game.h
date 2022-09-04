#pragma once

#include "../board.h"

struct GameSate {
  bool game_over;
  bool dumb_bool;
};

void common();

void horz_on_mouse_press();

void horz_on_mouse_release(Board *board, BoardState board_state);

void horz_update(Board *board, GameSate *game_state);
