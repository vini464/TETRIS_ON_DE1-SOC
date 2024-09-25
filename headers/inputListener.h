#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include <intelfpgaup/KEY.h>
#include <stdio.h>
#include <stdlib.h>
#include "accel.h"
#include "utils.h"

void startButtonListener();
void* buttonListener(void*);
void stopButtonListener();

void startAccelListener();
void* accelListener(void *);
void stopAccelListener();

#endif // !INPUT_LISTENER_H
