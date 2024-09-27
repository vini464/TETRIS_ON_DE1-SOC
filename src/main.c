#include "../headers/game.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../headers/accel.h"
#include "../headers/rendering.h"
#include "../headers/utils.h"
#include <intelfpgaup/HEX.h>
#include <intelfpgaup/KEY.h>
#include <intelfpgaup/video.h>

void startButtonListener();
void *buttonListener(void *);
void stopButtonListener();

void startAccelListener();
void *accelListener(void *);
void stopAccelListener();
int points();

void sm();
// void showMatrix(int height, int width, Color matrix[height][width]);
void hex_handler(int pts);

void game();
Color board[BOARDHEIGHT][BOARDWIDTH];
int BUTTON, PAUSED = 0, START = 0;
directions direction;
int LISTEN_BTN, LISTEN_ACCEL;

boolean FINISH = FALSE;
Piece ACTUAL_PIECE, NEXT_PIECE;

int main(void) {
  srand(time(NULL));
  LISTEN_BTN = 0;
  LISTEN_ACCEL = 0;
  int j, k;
  for (j = 0; j < BOARDHEIGHT; j++) {
    for (k = 0; k < BOARDWIDTH; k++) {
      board[j][k] = 0;
    }
  }
  int d;
  boolean collide = FALSE;
  directions dir;
  pthread_t btns, accel;
  startAccelListener();
  startButtonListener();
  video_open();
  pthread_create(&btns, NULL, buttonListener, NULL);
  pthread_create(&accel, NULL, accelListener, NULL);
  LISTEN_BTN = LISTEN_ACCEL = 1;
  while (1) {
    showTitle();
    while (!START) {
      if (BUTTON == 2) {
        START = 1;
        BUTTON = 0;
        printf(".");
      }
    }
        printf(".");
    game();
    START = 0;
    FINISH = FALSE;
  }
  LISTEN_ACCEL = 0;
  LISTEN_BTN = 0;

  pthread_exit(NULL);
  stopAccelListener();
  stopButtonListener();
  return 0;
}

void game() {
  int j, k;
  int pts = 0;
    hex_handler(pts);
  for (j = 0; j < BOARDHEIGHT; j++) {
    for (k = 0; k < BOARDWIDTH; k++)
      board[j][k] = 0;
  }
  int collide;
  while (FINISH == FALSE) {
    collide = FALSE;
    ACTUAL_PIECE = getPiece(rand() % 17);
    ACTUAL_PIECE.color = getColor(rand() % 9);
    while (!collide) {
      if (BUTTON == 1) {
        PAUSED = PAUSED ? 0 : 1;
        BUTTON = 0;
      }
      while (PAUSED) {
        if (BUTTON == 1) {
          PAUSED = PAUSED ? 0 : 1;
          BUTTON = 0;
        }
      }
      collide = movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, DOWN,
                          &FINISH);
      showMatrix(BOARDHEIGHT, BOARDWIDTH, board);

      usleep(200000);
    }
    gravity(BOARDHEIGHT, BOARDWIDTH, board);
    pts += points();
    hex_handler(pts);
  }
}
/**
void sm() {

  int j, k;
  system("clear");

  for (j = 0; j < BOARDHEIGHT; j++) {
    for (k = 0; k < BOARDWIDTH; k++) {
      // if (board[j][k] > 0) printf("[]");
      // else printf("  ");
      printf("%d  ", board[j][k]);
    }
    printf("\n");
  }
}
**/
int fd;
void startButtonListener() {
  int success, t;
  for (t = 0; t < 10; t++) {
    success = KEY_open();
    if (!success)
      printf("Não foi possível acessar o driver dos botões\n Você rodou como "
             "sudo?\n");
    else
      break;
  }
  if (!success)
    exit(-1);
  KEY_read(&t);
}

void *buttonListener(void *arg) {
  printf("lendo botao");
  int btn = 0, t;
  while (LISTEN_BTN) {
    // while (BUTTON == 0) {
    printf("BUTTON: %d\n", BUTTON);
    KEY_read(&btn);
    if (btn & 0b0001) {
      BUTTON = 1;
      printf("BUTTON: %d\n", BUTTON);
    } else if (btn & 0b0010) {
      BUTTON = 2;
      printf("BUTTON: %d\n", BUTTON);
    } else if (btn & 0b0100) {
      BUTTON = 3;
      printf("BUTTON: %d\n", BUTTON);
    } else if (btn & 0b1000) {
      BUTTON = 4;
      printf("BUTTON: %d\n", BUTTON);
    }
  }
}
void stopButtonListener() { KEY_close(); }

void startAccelListener() {
  int i;
  for (i = 0; i < 10; i++) {
    fd = open_and_mmap_dev_mem();
    if (fd == -1)
      printf("não foi possível abrir /dev/mem\n");
    else
      break;
  }

  if (fd == -1)
    exit(-1);
  I2C0_init();
  accel_init();
  accel_calibrate(60);
}
void *accelListener(void *arg) {
  int dir = 0;
  while (LISTEN_ACCEL) {
    get_direction(&dir);
    // printf("-%d\n", dir);
    switch (dir) {
    case -1:
      direction = LEFT;
      break;
    case 1:
      direction = RIGHT;
      break;
    default:
      direction = STOP;
    }
    if (direction != STOP && !PAUSED)
      movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, direction,
                &FINISH);
    direction = STOP;
    usleep(200000);
  }
}
void stopAccelListener() { close_and_unmap_dev_mem(fd); }

int points() {
  int j, pts = 0, combo = 0;
  while (1) {
    for (j = 0; j < BOARDWIDTH; j++) {
      if (board[BOARDHEIGHT - 1][j] == 0)
        return pts;
    }
    for (j = 0; j < BOARDWIDTH; j++) {
      board[BOARDHEIGHT - 1][j] = 0;
    }
    pts += 10 + combo * combo;
    combo++;
    gravity(BOARDHEIGHT, BOARDWIDTH, board);
  }
  return 10;
}

short colors[] = {video_GREEN, video_YELLOW, video_RED, video_BLUE, video_PINK};

void hex_handler(int pts) {
  HEX_open();
  HEX_set(pts);
  HEX_close();
}
