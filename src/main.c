#include "../headers/game.h"
#include "../headers/inputListener.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

Color board[BOARDHEIGHT][BOARDWIDTH];
extern int button;

boolean FINISH = FALSE;
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
  directions dir;
  pthread_t t;
  pthread_create(&t, NULL, buttonListener, NULL);
  while (!FINISH) {
    collide = FALSE;
    Piece bar = BAR;
    while (!collide && !FINISH) {
      sm();
    collide = movePiece(&bar, BOARDHEIGHT, BOARDWIDTH, board, DOWN, &FINISH);
     sm();
      if (button == 1) 
        collide = movePiece(&bar, BOARDHEIGHT, BOARDWIDTH, board, LEFT, &FINISH);
      if (button == 4) 
        collide = movePiece(&bar, BOARDHEIGHT, BOARDWIDTH, board, RIGHT, &FINISH);
      if (button == 2) 
        collide = movePiece(&bar, BOARDHEIGHT, BOARDWIDTH, board, DOWN, &FINISH);
      button = 0;
      usleep(100000);
      system("clear");
    }
    gravity(BOARDHEIGHT, BOARDWIDTH, board);
  }

  pthread_exit(NULL);
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
