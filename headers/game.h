#ifndef GAME_H
#define GAME_H

#define BOARDHEIGHT  20
#define BOARDWIDTH   10
#define SQUARES      4
#define FALSE        0
#define TRUE         1

typedef short int Color;
typedef struct pair {
  int first;
  int second;
} Pair;
typedef struct piece {
  Pair actual_pos[4];
  Pair older_pos[4];
  Color color;
} Piece;

typedef int bool;


typedef enum directions {
  LEFT = 1,
  DOWN = 2,
  RIGHT = 3
} directions;


/**
 * Esta função recebe a posição de um quadrado e verifica se alguma de suas coordenadas está fora
 * das dimensões da matriz ou se esta sobrepondo um quadrado já inserido na matriz
**/
bool checkCollision(Pair square_pos, int height, int width, Color board[height][width]);

/**
 * Esta função recebe a posição de um quadrado e insere 1 nas suas coordenadas na matriz;
**/
void insertInBoard(Pair square_pos, Color color, int height, int width, Color board[height][width]);

/**
 * Esta função recebe a posição de um quadrado e insere 0 nas suas coordenadas na matriz;
**/
void deleteInBoard(Pair square_pos, int height, int width, Color board[height][width]);

/**
 * Esta função recebe as posições da peça que está sendo inserida na matriz, caso
 * ela tenha algum dos seus blocos em uma coordenada negativa (fora do quadro)
 * é considerado game over e a função retorna 1, caso contrário 0 é retornado.
**/
int checkGameOver(Pair positions[SQUARES]);

/**
 * Esta função recebe uma peça e atualiza suas coordenadas uma linha abaixo. Até que não seja possível mover nenhum quadrado separadamente
**/
void movePieceDown(Piece *piece, int height, int width, Color board[height][width]);

/**
 * Esta função recebe uma peça e atualiza suas coordenadas uma coluna à esquerda.
**/
void movePieceLeft(Piece *piece);

/**
 * Esta função recebe uma peça e atualiza suas coordenadas uma coluna à direita.
**/
void movePieceRight(Piece *piece);

/**
 * Esta função rece o endereço de uma peça a direção que ela será movida na matriz,
 * ela move a peça na direção, remove a posição antiga da matriz e checa se a posição atual
 * colidiu, caso tenha, ela insere novamente a posição antiga e não atualiza a peça  - se a direção for para baixo ela chega se deu GAME OVER -
 * caso não tenha colidido ela insere a posição atual na matriz e autaliza a peça
**/
void movePiece(Piece *piece, int height, int width, Color board[height][width], directions direction);

#endif // !GAME_H