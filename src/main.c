#include "../headers/game.h"
#include "../headers/inputListener.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *handleMovement(void *);
void sm();

Color board[BOARDHEIGHT][BOARDWIDTH];
extern int button;
extern directions direction;

boolean FINISH = FALSE;
int LISTEN_BTN = 0, LISTEN_ACCEL = 0;
const Piece BAR = {.color = 0xFFFF,
                   .actual_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-2, 5}},
                   .older_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-2, 5}}};
Piece ACTUAL_PIECE;

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
  pthread_t btns, accel, h_mov;
  pthread_create(&btns, NULL, buttonListener, NULL);
  startAccelListener();
  startButtonListener();
  pthread_create(&accel, NULL, accelListener, NULL);
  pthread_create(&h_mov, NULL, handleMovement, NULL);
  LISTEN_BTN = LISTEN_ACCEL = 1;
  while (!FINISH) {
    collide = FALSE;
    ACTUAL_PIECE = BAR;
    while (!collide) {
      collide = movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, DOWN,
                          &FINISH);
      sm();
      button = 0;
      usleep(80000);
      system("clear");
    }
    gravity(BOARDHEIGHT, BOARDWIDTH, board);
  }
  LISTEN_ACCEL = 0;

  pthread_exit(NULL);
  stopAccelListener();
  stopButtonListener();

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

void *handleMovement(void *arg) {
  while (!FINISH) {
    if (direction != STOP) {
      movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, direction, &FINISH);
      sm();
    }
  }
}
