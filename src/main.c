#include <stdio.h>
#include "../headers/game.h"

Color board[BOARDHEIGHT][BOARDWIDTH];

Piece bar = {
  .color = 0xFFFF,
  .actual_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-1, 6}},
  .older_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-1, 6}}
};

int main(void) {
  int j,k;
  for (j=0; j<BOARDHEIGHT; j++){
    for (k=0; k<BOARDWIDTH; k++){
      board[j][k] = 0;
    }
  }

  while (!GAMEOVER)

  
  return 0;
}
