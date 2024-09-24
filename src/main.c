#include "../headers/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Color board[BOARDHEIGHT][BOARDWIDTH];

boolean finish = FALSE;
const Piece BAR = {.color = 0xFFFF,
                   .actual_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-2, 5}},
                   .older_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-2, 5}}};
void sm();
int main(void) {
  int j, k;
  for (j = 0; j < BOARDHEIGHT; j++) {
    for (k = 0; k < BOARDWIDTH; k++) {
      board[j][k] = 0;
    }
  }
  int d;
  boolean collide = FALSE;
  while (!finish) {
    collide = FALSE;
    Piece bar = BAR;
    while (!collide && !finish) {
      sm();
      collide = movePiece(&bar, BOARDHEIGHT, BOARDWIDTH, board, DOWN, &finish);
      usleep(50000);
      system("clear");
    }
    gravity(BOARDHEIGHT, BOARDWIDTH, board);
  }

  return 0;
}

void sm() {
  int j, k;
  printf("==--------------------==\n");
  for (j = 0; j < BOARDHEIGHT; j++) {
    for (k = 0; k < BOARDWIDTH; k++) {
      // if (board[j][k] > 0) printf("[]");
      // else printf("  ");
      printf("%d  ", board[j][k]);
    }
    printf("\n");
  }
  printf("==--------------------==");
}
