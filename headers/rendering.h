#ifndef REDENRING_H
#define REDENRING_H

#include "utils.h"
#include <stdio.h>

/**
 * Abre o arquivo para escrita no VGA, limpa o video e calcula o centro da tela.
 **/
void initScreen(int *width_center);
/**
 * Mostra na tela uma mensagem de início de jogo.
 **/
void initialScreen();
/**
 * Mostra na tela uma mensagem de Game Over.
 **/
void showGameOver();
/**
 * Mostra na tela a matriz do jogo.
 **/
void showMatrix(int height, int width, Color matrix[height][width], int paused,
                int width_center);
/**
 * Limpa a os dois buffers de desenho do VGA e o buffer de texto.
 **/
void clearVideo();
/**
 * Limpa o vídeo e fecha o arquivo para escrita no VGA
 **/
void shutdownScreen();

#endif // !REDENRING_H
