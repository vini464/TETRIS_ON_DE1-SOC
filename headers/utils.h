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

typedef enum directions { STOP = 0, LEFT = 1, DOWN = 2, RIGHT = 3 } directions;

const Piece BAR_HORIZONTAL = {
    .actual_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-1, 6}},
    .older_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-1, 6}}};

const Piece BAR_VERTICAL = {.actual_pos = {{-1, 5}, {-2, 5}, {-3, 5}, {-4, 5}},
                            .older_pos = {{-1, 5}, {-2, 5}, {-3, 5}, {-4, 5}}};

const Piece T_SHAPE_UP = {.actual_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-2, 4}},
                          .older_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-2, 4}}};

const Piece T_SHAPE_DOWN = {.actual_pos = {{-1, 4}, {-2, 3}, {-2, 4}, {-2, 5}},
                            .older_pos = {{-1, 4}, {-2, 3}, {-2, 4}, {-2, 5}}};

const Piece T_SHAPE_LEFT = {.actual_pos = {{-1, 4}, {-2, 3}, {-2, 4}, {-3, 4}},
                            .older_pos = {{-1, 4}, {-2, 3}, {-2, 4}, {-3, 4}}};

const Piece T_SHAPE_RIGHT = {.actual_pos = {{-1, 4}, {-2, 4}, {-2, 5}, {-3, 4}},
                             .older_pos = {{-1, 4}, {-2, 4}, {-2, 5}, {-3, 4}}};

const Piece L_SHAPE_DOWN1 = {.actual_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-2, 5}},
                             .older_pos = {{-1, 3}, {-1, 4}, {-1, 5}, {-2, 5}}};

const Piece L_SHAPE_DOWN2 = {.actual_pos = {{-1, 4}, {-2, 4}, {-2, 5}, {-2, 6}},
                             .older_pos = {{-1, 4}, {-1, 5}, {-1, 6}, {-2, 4}}};

const Piece L_SHAPE_UP1 = {.actual_pos = {{-1, 4}, {-1, 5}, {-2, 4}, {-3, 4}},
                           .older_pos = {{-1, 4}, {-1, 5}, {-2, 4}, {-3, 4}}};

const Piece L_SHAPE_UP2 = {.actual_pos = {{-1, 5}, {-2, 5}, {-3, 4}, {-3, 5}},
                           .older_pos = {{-1, 5}, {-2, 5}, {-3, 4}, {-3, 5}}};

const Piece J_SHAPE_UP1 = {.actual_pos = {{-1, 4}, {-1, 5}, {-2, 5}, {-3, 5}},
                           .older_pos = {{-1, 4}, {-1, 5}, {-2, 5}, {-3, 5}}};

const Piece J_SHAPE_UP2 = {.actual_pos = {{-1, 5}, {-2, 5}, {-3, 5}, {-3, 6}},
                           .older_pos = {{-1, 5}, {-2, 5}, {-3, 5}, {-3, 6}}};

const Piece J_SHAPE_DOWN1 = {.actual_pos = {{-1, 4}, {-1, 5}, {-1, 6}, {-2, 4}},
                             .older_pos = {{-1, 4}, {-1, 5}, {-1, 6}, {-2, 4}}};

const Piece J_SHAPE_DOWN2 = {.actual_pos = {{-1, 6}, {-2, 4}, {-2, 5}, {-2, 6}},
                             .older_pos = {{-1, 6}, {-2, 4}, {-2, 5}, {-2, 6}}};

const Piece S_SHAPE_HOR = {.actual_pos = {{-1, 4}, {-1, 5}, {-2, 5}, {-2, 6}},
                           .older_pos = {{-1, 4}, {-1, 5}, {-2, 5}, {-2, 6}}};

const Piece S_SHAPE_VER = {.actual_pos = {{-1, 5}, {-2, 4}, {-2, 5}, {-3, 4}},
                           .older_pos = {{-1, 5}, {-2, 4}, {-2, 5}, {-3, 4}}};

const Piece Z_SHAPE_HOR = {.actual_pos = {{-1, 5}, {-1, 6}, {-2, 4}, {-2, 5}},
                           .older_pos = {{-1, 5}, {-1, 6}, {-2, 4}, {-2, 5}}};

const Piece Z_SHAPE_VER = {.actual_pos = {{-1, 4}, {-2, 4}, {-2, 5}, {-3, 5}},
                           .older_pos = {{-1, 4}, {-2, 4}, {-2, 5}, {-3, 5}}};

const Piece SQUARE_SHAPE = {.actual_pos = {{-1, 5}, {-1, 6}, {-2, 5}, {-2, 6}},
                            .older_pos = {{-1, 5}, {-1, 6}, {-2, 5}, {-2, 6}}};

const Piece pieces[] = {
    BAR_VERTICAL,  T_SHAPE_UP,    T_SHAPE_DOWN, T_SHAPE_LEFT, T_SHAPE_RIGHT,
    L_SHAPE_DOWN1, L_SHAPE_DOWN2, L_SHAPE_UP1,  J_SHAPE_UP1,  J_SHAPE_UP2,
    J_SHAPE_DOWN1, J_SHAPE_DOWN2, S_SHAPE_HOR,  S_SHAPE_VER,  Z_SHAPE_HOR,
    Z_SHAPE_VER,   SQUARE_SHAPE};
const Color colors[] = {0xFFE0, 0xF800, 0x07E0, 0x041F, 0x07FF,
                        0xF81F, 0xC618, 0xFC18, 0xFC00};

#endif // !UTILS_H
