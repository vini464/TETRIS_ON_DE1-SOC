#include "../headers/game.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>



#include <intelfpgaup/KEY.h>
#include <intelfpgaup/video.h>
#include <intelfpgaup/HEX.h>
#include "../headers/accel.h"
#include "../headers/utils.h"

void startButtonListener();
void* buttonListener(void*);
void stopButtonListener();

void startAccelListener();
void* accelListener(void *);
void stopAccelListener();
int points();


void sm();
void showMatrix(int height, int width, Color matrix[height][width]);
Color board[BOARDHEIGHT][BOARDWIDTH];
int button, paused =0;
directions direction;
int LISTEN_BTN, LISTEN_ACCEL;

boolean FINISH = FALSE;
Piece ACTUAL_PIECE, NEXT_PIECE;

int main(void) {
  srand(time(NULL));
  LISTEN_BTN = 0; LISTEN_ACCEL = 0;
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
  startAccelListener();
  startButtonListener();
  pthread_create(&btns, NULL, buttonListener, NULL);
  pthread_create(&accel, NULL, accelListener, NULL);
  int pts = 0;
  LISTEN_BTN = LISTEN_ACCEL = 1;
  while (FINISH == FALSE) {
    collide = FALSE;
    ACTUAL_PIECE = pieces[rand() % 17];
    ACTUAL_PIECE.color = colors[rand() % 9];
    while (!collide) {
      if (button == 1){
        paused = paused ? 0 : 1;
        button =0;
      }
      while(paused){
        if (button == 1){
        paused = paused ? 0 : 1;
        button =0;
      }
      }
      if (button == 1 ){
      collide = movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, LEFT,&FINISH);
      button = 0;

      }
      
      collide = movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, DOWN,&FINISH);
      showMatrix(BOARDHEIGHT, BOARDWIDTH, board);

      usleep(200000);
    }
    gravity(BOARDHEIGHT, BOARDWIDTH, board);
    pts += points();
    hex_handler(pts);
  }
  LISTEN_ACCEL = 0;
  LISTEN_BTN = 0;
  printf("points: %d\n", pts);

  pthread_exit(NULL);
  stopAccelListener();
  stopButtonListener();
  return 0;
}

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

int fd, button = 0;
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
}

void *buttonListener(void* arg) {
  printf("lendo botao");
  while (LISTEN_BTN) {
  int btn = 0;
    KEY_read(&btn);
    switch (btn) {
    case 0b0001:
      button = 1;
      break;
    case 0b0010:
      button = 2;
      break;
    case 0b0100:
      button = 3;
      break;
    case 0b1000:
      button = 4;
      break;

    }
  }
}
void stopButtonListener(){
  KEY_close();
}

void startAccelListener(){
  int  i;
  for (i = 0; i < 10; i++){
    fd = open_and_mmap_dev_mem();
    if (fd == -1) printf("não foi possível abrir /dev/mem\n");
    else break;
  }
  
  if (fd == -1) exit(-1);
  I2C0_init();
  accel_init();
  accel_calibrate(60);

}
void *accelListener(void * arg){
  int dir = 0;
  while (LISTEN_ACCEL) {
    get_direction(&dir);
    //printf("-%d\n", dir);
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
    if (direction != STOP && !paused)
      movePiece(&ACTUAL_PIECE, BOARDHEIGHT, BOARDWIDTH, board, direction, &FINISH);
    direction = STOP;
    usleep(200000);
  }
}
void stopAccelListener(){
      close_and_unmap_dev_mem(fd);
}

int points() {
  int j, pts =0, combo=0;
  while (1){
  for (j=0; j<BOARDWIDTH; j++){
    if (board[BOARDHEIGHT-1][j] == 0) return pts;
  }
  for (j=0; j<BOARDWIDTH; j++){
    board[BOARDHEIGHT-1][j] = 0;
  }
  pts+=10 + combo*combo;
  combo++;
  gravity(BOARDHEIGHT, BOARDWIDTH, board);
  showMatrix(BOARDHEIGHT, BOARDWIDTH, board);
  }
  return 10;
}


short colors[] = {video_GREEN, video_YELLOW, video_RED, video_BLUE, video_PINK};
void showMatrix(int height, int width, Color matrix[height][width]) {
  int lines, cols, tlines, tcols, mid, offset;
  Pair p1;
  // preparando a tela
  video_open();
  video_read(&lines, &cols, &tlines, &tcols);
  mid = cols/2;
  p1.first = 25;
  p1.second =  mid-9;
  offset =7;
  int l_off = (tlines-p1.first+(offset*20)+(19*2))/2;
  // draw a rect:
  video_clear();
  video_erase();
  video_box(p1.second, p1.first, p1.second+(offset*10)+(9*2), p1.first+(offset*20)+(19*2), 0xFFFF);

  // desenhando as peças:
  int l, c;
  for (l=0; l<height; l++){
	  p1.second =  mid-9;
    for (c=0; c<width; c++){
      if (matrix[l][c] > 0){
	//printf("has_block\ncolor: %d\n", matrix[l][c]); 
        video_box(p1.second, p1.first, p1.second+offset, p1.first+offset, matrix[l][c]);
      }
      p1.second += offset+2;
    }
      p1.first += offset + 2;
  } 
  video_show();

}

void hex_handler(int pts){
  HEX_open();
  HEX_set(pts);
  HEX_close();
}
