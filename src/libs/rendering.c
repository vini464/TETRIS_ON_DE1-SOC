#include "../../headers/rendering.h"

void initScreen(int *qnt_size, int *d_width_center, int *t_width_center) {
  int lines, cols, tlines, tcols, mid;
  while (video_open() == 0) {
    printf("Não foi possível abrir o driver de vídeo\nVocê está rodando como "
           "sudo?\n");
  }
  clearVideo();
  video_read(&lines, &cols, &tlines, &tcols);
  *qnt_size = cols / 5;
  *t_width_center = tcols/2;
  *d_width_center = cols/2;
  printf("tcols: %d\n", tcols);
  
}

void clearVideo() {
  video_clear();
  video_show();
  video_clear();
  video_erase();
}

void shutdownScreen() {
  clearVideo();
  video_close();
}


void showMatrix(int height, int width, Color matrix[height][width], int points, int paused,int width_center) {
  int lines, cols, tlines, tcols, mid;
  Pair p1;
  // preparando a tela
  video_read(&lines, &cols, &tlines, &tcols);
  int initial_point = width_center - ((4 * BOARDWIDTH) + 2)/2;
  int end_point = width_center + ((4 * BOARDWIDTH) + 2)/2;

  mid = cols / 2;
    p1.second = initial_point;
    p1.first = 0;
  // draw a rect:
 video_clear();
 video_box(p1.second, p1.first, p1.second + (4 * 10) + (9 * 2) + 4,
            p1.first + (4 * 20) + (19 * 2), 0xFFFF);

  // desenhando as peças:
  p1.first = 0;
  int l, c;
  for (l = 0; l < height; l++) {
    p1.second = initial_point+4;
    for (c = 0; c < width; c++) {
      if (matrix[l][c] > 0) {
        video_box(p1.second, p1.first, p1.second + 4, p1.first + 4,
                  matrix[l][c]);
      }
      p1.second += 4 + 2;
    }
    p1.first += 4 + 2;
  }
  video_show();
}

void initialScreen(int width_center){
  char *name[] = {
   "___________        ___________        .__ __________",
   "\\__    ___/  ____  \\__    ___/_______ |__|\\____    /",
   "  |    |   _/ __ \\   |    |   \\_  __ \\|  |  /     / ",
   "  |    |   \\  ___/   |    |    |  | \\/|  | /     /_ ",
   "  |____|    \\___  >  |____|    |__|   |__|/_______ \\",
   "                \\/                                \\/",
   "       Pressione qualquer botao para iniciar        "
  };
  int l;
  Pair p;
  p.first = 5;
  p.second = 14;
  for (l=0; l<7; l++)
    video_text(p.second, p.first+l, name[l]);

  printf("acabou\n");
}

void showGameOver() {
  char *name[] = {
"  ______    ______   __       __  ________ ",
" /      \\  /      \\ |  \\     /  \\|        \\",
"|  $$$$$$\\|  $$$$$$\\| $$\\   /  $$| $$$$$$$$",
"| $$ __\\$$| $$__| $$| $$$\\ /  $$$| $$__    ",
"| $$|    \\| $$    $$| $$$$\\  $$$$| $$  \\   ",
"| $$ \\$$$$| $$$$$$$$| $$\\$$ $$ $$| $$$$$   ",
"| $$__| $$| $$  | $$| $$ \\$$$| $$| $$_____ ",
" \\$$    $$| $$  | $$| $$  \\$ | $$| $$     \\",
"  \\$$$$$$  \\$$   \\$$ \\$$      \\$$ \\$$$$$$$$",
"  ______   __     __  ________  _______    ",
" /      \\ |  \\   |  \\|        \\|       \\   ",
"|  $$$$$$\\| $$   | $$| $$$$$$$$| $$$$$$$\\  ",
"| $$  | $$| $$   | $$| $$__    | $$__| $$  ",
"| $$  | $$ \\$$\\ /  $$| $$  \\   | $$    $$  ",
"| $$  | $$  \\$$\\  $$ | $$$$$   | $$$$$$$\\  ",
"| $$__/ $$   \\$$ $$  | $$_____ | $$  | $$  ",
" \\$$    $$    \\$$$   | $$     \\| $$  | $$  ",
"  \\$$$$$$      \\$     \\$$$$$$$$ \\$$   \\$$  ",
 "Pressione o botao um para sair e qualquer outro para reiniciar" };

                                           
  int l;
  Pair p;
  p.first = 5;
  p.second = 14;
clearVideo();
  for (l=0; l<19; l++)
    video_text(p.second, p.first+l, name[l]);

  printf("acabou\n");
}
