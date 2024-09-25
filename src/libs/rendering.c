#include "../../headers/rendering.h"
#include <intelfgaup/video.h>

void showTitle(){
  int lines, cols, tlines, tcols, mid, offset;
  Pair p1;
  // preparando a tela
  video_read(&lines, &cols, &tlines, &tcols);
  mid = cols / 2;
  p1.first = 25;
  p1.second = mid - 9;
  offset = 7;
  int l_off = (tlines - p1.first + (offset * 20) + (19 * 2)) / 2;
  // draw a rect:
  video_clear();
  video_erase();
  video_box(p1.second, p1.first, p1.second + (offset * 10) + (9 * 2),
            p1.first + (offset * 20) + (19 * 2), 0xFFFF);
  video_text(tcols/2, tlines/2, "Aperte o botão um para continuar");
}
void showGameOver(){}
void showMenu(int argc, char *argv[]){}

void showMatrix(int height, int width, Color matrix[height][width]) {
  int lines, cols, tlines, tcols, mid, offset;
  Pair p1;
  // preparando a tela
  video_read(&lines, &cols, &tlines, &tcols);
  mid = cols / 2;
  p1.first = 25;
  p1.second = mid - 9;
  offset = 7;
  int l_off = (tlines - p1.first + (offset * 20) + (19 * 2)) / 2;
  // draw a rect:
  video_clear();
  video_erase();
  video_box(p1.second, p1.first, p1.second + (offset * 10) + (9 * 2),
            p1.first + (offset * 20) + (19 * 2), 0xFFFF);

  // desenhando as peças:
  int l, c;
  for (l = 0; l < height; l++) {
    p1.second = mid - 9;
    for (c = 0; c < width; c++) {
      if (matrix[l][c] > 0) {
        // printf("has_block\ncolor: %d\n", matrix[l][c]);
        video_box(p1.second, p1.first, p1.second + offset, p1.first + offset,
                  matrix[l][c]);
      }
      p1.second += offset + 2;
    }
    p1.first += offset + 2;
  }
  video_show();
}
