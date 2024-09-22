#include <stdio.h>
#include "../headers/game.h"

Color board[BOARDHEIGHT][BOARDWIDTH];
bool GAMEOVER = FALSE;
Piece bar = {
  .color = 0xFFFF,
  .actual_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-1, 6}},
  .older_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-1, 6}}
};
void sm();
int main(void) {
  int j,k;
  for (j=0; j<BOARDHEIGHT; j++){
    for (k=0; k<BOARDWIDTH; k++){
      board[j][k] = 0;
    }
  }

  while (!GAMEOVER){
    movePiece(&bar, BOARDHEIGHT, BOARDWIDTH, board, DOWN);
    GAMEOVER = checkGameOver(bar.actual_pos);
  }

  
  return 0;
}

void sm(){
  int j, k;
  for (j=0; j<BOARDHEIGHT; j++){
    for (k=0; k<BOARDWIDTH; k++){
      if (board[j][k] > 0) printf("[]");
      else printf("  ");
      printf(" ");
    }
    printf("\n");
  }
}
