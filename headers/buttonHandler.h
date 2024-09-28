#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

/**
 * Abre o arquivo para leitura dos botões e faz uma leitura para limpar o arquivo.
 **/
void startButton();
/**
 * Verifica se apenas um botão foi pressionado e retorna o número correspondente aquela botão.
 **/
int readSingleButton();
/**
 * Faz uma leitura para limpar o arquivo dos botões e o fecha em seguida.
 **/
void stopButton();

#endif // !BUTTONHANDLER_H
