#ifndef UTILS_H
#define UTILS_H

#define BOARDHEIGHT 20
#define BOARDWIDTH 10
#define SQUARES 4

typedef unsigned short int Color;
typedef struct pair {
  int first;
  int second;
} Pair;
typedef struct piece {
  Pair actual_pos[4];
  Pair older_pos[4];
  Color color;
} Piece;

typedef enum boolean { TRUE = 1, FALSE = 0 } boolean;

typedef enum directions { STOP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 } Direction;

Color getColor(int index);
Piece getPiece(int index);

#endif // !UTILS_H
