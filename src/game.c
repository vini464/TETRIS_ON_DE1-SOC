#include "../headers/game.h"

int GAMEOVER = 0;

void movePieceDown(Piece *piece, int height, int width, Color board[height][width]){
  int j;
  bool collide;
  for (j=0; j<SQUARES; j++){
    piece->older_pos[j].first = piece->actual_pos[j].first;
    piece->actual_pos[j].first ++;
    // gravity:
    if (checkCollision(piece->actual_pos[j], height, width, board)){
      // volta para a posição anterior
      piece->actual_pos[j].first--;
      piece->older_pos[j].first--;

    }
  }
}

void movePieceLeft(Piece *piece){
  int j;
  for (j=0; j<SQUARES; j++){
    piece->older_pos[j].second = piece->actual_pos[j].second;
    piece->actual_pos[j].first--;
  }
}

void movePieceRight(Piece *piece){
  int j;
  for (j=0; j<SQUARES; j++){
    piece->older_pos[j].second = piece->actual_pos[j].second;
    piece->actual_pos[j].second ++;
  }
}

void deleteInBoard(Pair square_pos, int height, int width, Color board[height][width]){
    if (square_pos.first >= 0 && square_pos.second >= 0){
      board[square_pos.first][square_pos.second] = 0;
    }
}

void insertInBoard(Pair square_pos, Color color, int height, int width, Color board[height][width]) {
    if (square_pos.first >= 0 && square_pos.second >= 0){
      board[square_pos.first][square_pos.second] = 1;
    }
}


bool checkCollision(Pair square_pos, int height, int width, Color board[height][width]){
  int j, k;
    if (square_pos.first >= height || square_pos.second < 0 || square_pos.second >= width || (square_pos.first >= 0 && board[square_pos.first][square_pos.second])) 
      return TRUE;
  return FALSE;
}


void movePiece(Piece *piece, int height, int width, Color board[height][width], directions direction){
  Piece new_pos = *piece;
  bool collide;
  
  switch (direction) {
    case LEFT: 
      movePieceLeft(&new_pos);
      break;
    case RIGHT:
      movePieceRight(&new_pos);
      break;
    case DOWN:
      movePieceDown(&new_pos, height, width, board);
      break;
  }
  
  int j;
  for (j=0; j<SQUARES; j++)
    deleteInBoard(new_pos.older_pos[j], height, width, board);

  for (j=0; j<SQUARES; j++){ // checa se ao menos um quadrado colidiu

    collide = checkCollision(new_pos.actual_pos[j], height, width, board);
    if (collide) break; 
  }

  if (!collide){
    for (j=0; j<SQUARES; j++)
      insertInBoard(new_pos.actual_pos[j], new_pos.color, height, width, board);
    *piece = new_pos;
    return;
  }
  else{
    for (j=0; j<SQUARES; j++)
      insertInBoard(new_pos.older_pos[j], new_pos.color, height, width, board);
    if (collide && direction == DOWN){
      checkGameOver(new_pos.actual_pos);
    }
  }
  
}
