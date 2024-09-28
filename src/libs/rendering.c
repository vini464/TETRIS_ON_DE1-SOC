#include "../../headers/rendering.h"
#include <intelfpgaup/video.h>

void initScreen(int *width_center) {
  int lines, cols, tlines, tcols, mid;
  while (video_open() == 0) {
    printf("Não foi possível abrir o driver de vídeo\nVocê está rodando como "
           "sudo?\n");
  }
  clearVideo();
  video_read(&lines, &cols, &tlines, &tcols);
  *width_center = cols / 2;
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

void showMatrix(int height, int width, Color matrix[height][width], int paused,
                int width_center) {
  int lines, cols, tlines, tcols, mid;
  Pair p1;
  // preparando a tela
  video_read(&lines, &cols, &tlines, &tcols);
  int initial_point = width_center - ((7 * BOARDWIDTH)) / 2;
  int end_point = width_center + ((7 * BOARDWIDTH)) / 2;

  mid = cols / 2;
  p1.second = initial_point;
  p1.first = 0;
  // draw a rect:
  video_clear();
  video_box(p1.second, p1.first, p1.second + (7 * 10) + (9 * 2),
            p1.first + (7 * 20) + (19 * 2), 0xFFFF);

  // desenhando as peças:
  p1.first = 0;
  int l, c;
  for (l = 0; l < height; l++) {
    p1.second = initial_point;
    for (c = 0; c < width; c++) {
      if (matrix[l][c] > 0) {
        video_box(p1.second, p1.first, p1.second + 7, p1.first + 7,
                  matrix[l][c]);
      }
      p1.second += 7 + 2;
    }
    p1.first += 7 + 2;
  }
  video_show();
}

void initialScreen() {
  char *name[] = {"___________        ___________        .__ __________",
                  "\\__    ___/  ____  \\__    ___/_______ |__|\\____    /",
                  "  |    |   _/ __ \\   |    |   \\_  __ \\|  |  /     / ",
                  "  |    |   \\  ___/   |    |    |  | \\/|  | /     /_ ",
                  "  |____|    \\___  >  |____|    |__|   |__|/_______ \\",
                  "                \\/                                \\/",
                  "       Pressione qualquer botao para iniciar        "};
  int l;
  Pair p;
  p.first = 5;
  p.second = 14;
  for (l = 0; l < 7; l++)
    video_text(p.second, p.first + l, name[l]);
}

void showGameOver() {
  char *name[] = {"  ______    ______   __       __  ________ ",
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
                  "        Pressione o botao 0 para sair",
                  "        Qualquer outro para reiniciar"};

  int l;
  Pair p;
  p.first = 5;
  p.second = 14;
  clearVideo();
  for (l = 0; l < 20; l++)
    video_text(p.second, p.first + l, name[l]);
}
