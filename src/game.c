#include "../headers/game.h"

int GAMEOVER = 0;

void movePieceDown(Piece *piece){
  int j;
  for (j=0; j<SQUARES; j++){
    piece->older_pos[j].first = piece->actual_pos[j].first;
    piece->older_pos[j].second = piece->actual_pos[j].second;
    piece->actual_pos[j].first ++;
  }
}

void movePieceLeft(Piece *piece){
  int j;
  for (j=0; j<SQUARES; j++){
    piece->older_pos[j].first = piece->actual_pos[j].first;
    piece->older_pos[j].second = piece->actual_pos[j].second;
    piece->actual_pos[j].first--;
  }
}

void movePieceRight(Piece *piece){
  int j;
  for (j=0; j<SQUARES; j++){
    piece->older_pos[j].first = piece->actual_pos[j].first;
    piece->older_pos[j].second = piece->actual_pos[j].second;
    piece->actual_pos[j].second ++;
  }
}

void deleteInBoard(Pair positions[SQUARES], int height, int width, Color board[height][width]){
  int j;
  for (j = 0; j < SQUARES; j++){
    Pair pos = positions[j];
    if (pos.first >= 0 && pos.second >= 0){
      board[pos.first][pos.second] = 0;
    }
  }
}

void insertInBoard(Pair positions[SQUARES], Color color, int height, int width, Color board[height][width]) {
  int j;
  for (j = 0; j < SQUARES; j++){
    Pair pos = positions[j];
    if (pos.first >= 0 && pos.second >= 0){
      board[pos.first][pos.second] = color;
    }
  }
}


bool checkCollision(Pair positions[SQUARES], int height, int width, Color board[height][width]){
  int j, k;
  for (j = 0; j < SQUARES; j++){
    Pair pos = positions[j];
    if (pos.first >= height || pos.second < 0 || pos.second >= width || (pos.first >= 0 && board[pos.first][pos.second])) 
      return TRUE;
  }
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
      movePieceDown(&new_pos);
      break;
  }
  
  deleteInBoard(new_pos.older_pos, height, width, board);
  collide = checkCollision(new_pos.actual_pos, height, width, board);
  if (!collide){
    insertInBoard(new_pos.actual_pos, new_pos.color, height, width, board);
    *piece = new_pos;
    return;
  }
  else{
    insertInBoard(new_pos.older_pos, new_pos.color, height, width, board);
    if (collide && direction == DOWN){
      checkGameOver(new_pos.actual_pos);
    }
  }
  
}
