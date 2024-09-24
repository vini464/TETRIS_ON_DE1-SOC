#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include <intelfpgaup/KEYS.h>
#include <stdio.h>
#include <stdlib.h>


void startButtonListener();
void* buttonListener(void*);
void stopButtonListener();

void startAccelListener();
void accelListener(void *);

#endif // !INPUT_LISTENER_H
