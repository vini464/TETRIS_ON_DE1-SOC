#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../headers/accel.h"
#include "../headers/buttonHandler.h"
#include "../headers/game.h"
#include "../headers/hexHandler.h"
#include "../headers/rendering.h"
#include "../headers/utils.h"

void *buttonListener(void *);

void startAccelListener();
void *accelListener(void *);
void stopAccelListener();

void game();

Color BOARD[BOARDHEIGHT][BOARDWIDTH];
int LISTEN_BTN, LISTEN_ACCEL, BUTTON, FD, WIDTH_CENTER, PTS;
boolean GAMEOVER, PAUSED = FALSE, OUT = FALSE;
Piece ACTUAL_PIECE, NEXT_PIECE;

int main(void) {
  srand(time(NULL)); // Definir uma seed "aleatória" para o rand
  int d, reset = 0, start = 0;
  boolean collide = FALSE;
  pthread_t btns_t, accel_t;

  startAccelListener();
  startButton();
  startHex();
  initScreen(&WIDTH_CENTER);
  LISTEN_BTN = LISTEN_ACCEL = 1;
  pthread_create(&btns_t, NULL, buttonListener, NULL);
  pthread_create(&accel_t, NULL, accelListener, NULL);

  while (1) {
    BUTTON = 0;

    if (!start) {
      initialScreen();
      while (!start) {
        if (BUTTON != 0) {
          start = 1;
          BUTTON = 0;
        }
      }
    }

    clearVideo();
    game();
    usleep(1000);

    if (GAMEOVER) {
      showGameOver();
      BUTTON = 0;
      while (BUTTON == 0) {
        if (BUTTON == 1) {
          reset = 0;
        } else if (BUTTON > 1) {
          reset = 1;
          hexWriteNumber(0);
        }
      }
      if (!reset)
        break;
    } else {
      if (OUT)
        break;
      hexWriteNumber(0);
      GAMEOVER = FALSE;
    }
  }
  LISTEN_ACCEL = 0;
  LISTEN_BTN = 0;
  pthread_join(btns_t, NULL);
  pthread_join(accel_t, NULL);
  stopAccelListener();
  stopButton();
  clearVideo();
  stopHex();
  return 0;
}

void game() {
  int j, k;
  PTS = 0;
  hexWriteNumber(PTS);
  GAMEOVER = FALSE;
  for (j = 0; j < BOARDHEIGHT; j++) {
    for (k = 0; k < BOARDWIDTH; k++)
      BOARD[j][k] = 0;
  }
  int collide;
  while (GAMEOVER == FALSE) {
    collide = FALSE;
    ACTUAL_PIECE = getPiece(rand() % 17);
    ACTUAL_PIECE.color = getColor(rand() % 9);
    while (!collide) {
      if (BUTTON == 1) {
        PAUSED = TRUE;
        BUTTON = 0;
      }
      while (PAUSED) {
        if (BUTTON > 0 && BUTTON < 4) {
          PAUSED = FALSE;
          BUTTON = 0;
          if (BUTTON == 2) {
            return;
          }
          if (BUTTON == 3) {
            OUT = 1;
            return;
          }
        }
      }
      collide = movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, BOARD, DOWN,
                          &GAMEOVER);

      showMatrix(BOARDHEIGHT, BOARDWIDTH, BOARD, PAUSED, WIDTH_CENTER);

      usleep(100000);
    }
    gravity(BOARDHEIGHT, BOARDWIDTH, BOARD);
    PTS += clearLine(BOARDWIDTH, BOARDHEIGHT, BOARD);
    hexWriteNumber(PTS);
  }
}

void *buttonListener(void *arg) {
  int btn = 0, t;
  while (LISTEN_BTN) {
    BUTTON = readSingleButton();
  }
  return NULL;
}

void startAccelListener() {
  int i;
  for (i = 0; i < 10; i++) {
    FD = open_and_mmap_dev_mem();
    if (FD == -1)
      printf("não foi possível abrir /dev/mem\n");
    else
      break;
  }
  if (FD == -1)
    exit(-1);
  I2C0_init();
  accel_init();
  accel_calibrate(60);
}
void *accelListener(void *arg) {
  int accel_read = 0;
  Direction dir;
  while (LISTEN_ACCEL) {
    get_direction(&accel_read);
    switch (accel_read) {
    case -1:
      dir = LEFT;
      break;
    case 1:
      dir = RIGHT;
      break;
    default:
      dir = STOP;
    }
    if (dir != STOP && !PAUSED && !GAMEOVER) {

      movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, BOARD, dir, &GAMEOVER);
      showMatrix(BOARDHEIGHT, BOARDWIDTH, BOARD, PAUSED, WIDTH_CENTER);
    }
    dir = STOP;
    usleep(200000);
  }
  return NULL;
}
void stopAccelListener() { close_and_unmap_dev_mem(FD); }
