#include "../../headers/inputListener.h"

int button = 0;
extern int FINISH;
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
  int btn = 0;
  while (!FINISH) {
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
    default:
      button = 0;
    }
  }
}
void stopButtonListener(){
  KEY_close();
}

void startAccelListener();
void accelListener(void *direction);
