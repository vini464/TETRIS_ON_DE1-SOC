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
void hex_handler(int pts);

void game();

Color board[BOARDHEIGHT][BOARDWIDTH];
Direction direction;
int LISTEN_BTN, LISTEN_ACCEL, BUTTON, PAUSED = 0, START = 0, FD, square_size,
                                      d_width_center, t_width_center, pts, OUT = 0;
boolean FINISH = TRUE;
Piece ACTUAL_PIECE, NEXT_PIECE;

int main(void) {
  srand(time(NULL)); // Definir uma seed "aleatória" para o rand
  LISTEN_BTN = 0;
  LISTEN_ACCEL = 0;
  int d;
  boolean collide = FALSE;
  Direction dir;
  pthread_t btns, accel;
  startAccelListener();
  startButtonListener();
  initScreen(&square_size, &d_width_center, &t_width_center);
  LISTEN_BTN = LISTEN_ACCEL = 1;
  pthread_create(&btns, NULL, buttonListener, NULL);
  pthread_create(&accel, NULL, accelListener, NULL);
  int reset = 0;
  while (1) {

    if (!START)
      initialScreen(d_width_center);
    
    while (!START) {
      if (BUTTON != 0) {
        START = 1;
        BUTTON = 0;
      }
    }
    clearVideo();
    game();
    usleep(1000);
    if (FINISH){
	    showGameOver();
	    BUTTON = 0;
	    START = 1;
	    while (BUTTON == 0) {
		    if (BUTTON == 1) {
			    reset = 0;
		    }
		    else if (BUTTON > 1) {
			    reset = 1;
			    hex_handler(0);
		    }
	    }
	    BUTTON = 0;
	    if (!reset) break; 
    }
   else {
if (OUT) break;
	   BUTTON = 0;
	   START = 1;
	   hex_handler(0);
	   FINISH = FALSE;

   }
  }
  LISTEN_ACCEL = 0;
  LISTEN_BTN = 0;
  clearVideo();
  HEX_open();
  HEX_raw(0b0, 0b0);
  HEX_close();
  pthread_exit(NULL);
  stopAccelListener();
  stopButtonListener();
  return 0;
}

void game() {
  int j, k;
  pts = 0;
  hex_handler(pts);
  FINISH = FALSE;
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
   	if (BUTTON == 2) { BUTTON = 0;PAUSED = 0; return;}
        if (BUTTON == 3) {BUTTON = 0; PAUSED =0; OUT=1; return;}
      }
      collide = movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, DOWN,
                          &FINISH);
     
      showMatrix(BOARDHEIGHT, BOARDWIDTH, board, pts, PAUSED, d_width_center);

      usleep(100000);
    }
    gravity(BOARDHEIGHT, BOARDWIDTH, board);
    pts += points();
    hex_handler(pts);
  }
}

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
  int btn = 0, t;
  while (LISTEN_BTN) {
    // while (BUTTON == 0) {
    KEY_read(&btn);
    if (btn & 0b0001) {
      BUTTON = 1;
    } else if (btn & 0b0010) {
      BUTTON = 2;
    } else if (btn & 0b0100) {
      BUTTON = 3;
    } else if (btn & 0b1000) {
      BUTTON = 4;
    }
  }
}
void stopButtonListener() { KEY_close(); }

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
    if (direction != STOP && !PAUSED && !FINISH) {

      movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, direction,
                &FINISH);
      showMatrix(BOARDHEIGHT, BOARDWIDTH, board, pts, PAUSED, d_width_center);
    }
    direction = STOP;
    usleep(200000);
  }
}
void stopAccelListener() { close_and_unmap_dev_mem(FD); }

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

void hex_handler(int pts) {

  HEX_open();
  if (pts > 999999) pts = 999999;
  HEX_set(pts);
  HEX_close();
}
