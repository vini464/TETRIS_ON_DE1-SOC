#ifndef REDENRING_H
#define REDENRING_H

#include "utils.h"
#include <intelfpgaup/video.h>
#include <stdio.h>

//void showTitle(int square_size, int offset);
void showGameOver();
void showMatrix(int height, int width, Color matrix[height][width], int points, int paused, int width_center);
void clearVideo();
void initScreen(int *square_size, int * d_width_center, int *t_width_center);
void shutdownScreen();
void initialScreen(int width_center);


#endif // !REDENRING_H
