#ifndef REDENRING_H
#define REDENRING_H

#include "utils.h"
#include <stdio.h>

void showGameOver();
void showMatrix(int height, int width, Color matrix[height][width], int paused, int width_center);
void clearVideo();
void initScreen(int * width_center);
void shutdownScreen();
void initialScreen();


#endif // !REDENRING_H
