#include "../../headers/hexHandler.h"
#include <intelfpgaup/HEX.h>

void startHex() {
  int success;
  HEX_open();
  clearHex();
}
void stopHex() {
  clearHex();
  HEX_close();
}
void clearHex() {
  HEX_raw(0b0, 0b0);
}
void hexWriteNumber(int number) {
  if (number > 999999) number = 999999;
  HEX_set(number);
}
