#include "../../headers/buttonHandler.h"
#include <intelfpgaup/KEY.h>


void startButton() {
  int trash;
  KEY_open();
  KEY_read(&trash);
}
int readSingleButton() {
  int btn = 0;
  KEY_read(&btn);
  if (btn == 1) return 1;
  if (btn == 2) return 2;
  if (btn == 4) return 3;
  if (btn == 8) return 4;

  return 0;
}
void stopButton() {
  int trash;
  KEY_read(&trash);
  KEY_close();
}
