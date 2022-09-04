#pragma once

#include "../board.h"

struct GameSate {
  bool game_over;
  bool dumb_bool;
};

void common();

void vert_on_mouse_press();

void vert_on_mouse_release(Board *board, BoardState board_state);

void vert_update(Board *board, GameSate *game_state);
