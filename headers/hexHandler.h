#ifndef HEXhANDLER_H
#define HEXhANDLER_H


/**
 * Abre o arquivo para escrita no display de sete segmentos.
 **/
void startHex();
/**
 * Limpa o display e fecha o arquivo.
 **/
void stopHex();
/**
 * Limpa o display de sete segmentos.
 **/
void clearHex();
/**
 * Escreve no display um número até 999999.
 **/
void hexWriteNumber(int number);

#endif // !HEXhANDLER_H
