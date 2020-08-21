#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define col 255

// Method Declarations
void print_pixmap8(uint8_t *pixmap, int width, int height);
void fail(char* s);
void change_color(int cur_bar, uint8_t *pixmap);
int main(int argc, char *argv[]);

uint8_t bar_color[] = {
  col, col, col,
  col, col,   0,
    0, col, col,
    0, col,   0,
  col,   0, col,
  col,   0,   0,
    0,   0, col
};

// print pixmap
void print_pixmap8(uint8_t *pixmap, int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uint8_t *pixel = &pixmap[4*((y * width) + x)];
      printf("#%02x%02x%02x ", pixel[0], pixel[1], pixel[2]);
    }
    printf("\n");
  }
}

// fail case
void fail(char* s) {
  printf("Error: %s\n\n", s);
  printf("Usage:\n");
  printf("test_pattern WIDTH HEIGHT\n\n");
  exit(1);
}

// main function
int main(int argc, char *argv[]) {

  //  incorrect amount of input by user
  if (argc != 3) {
    fail("Wrong number of arguments.");
  }

  int bar_width = atoi(argv[1]);
  int width = bar_width * 7;
  int height = atoi(argv[2]);

  // invalid input by user
  if(width <= 0 || height <= 0){
    fail("Height or Width non-positive integer");
  }

  // create pixmap
  uint8_t *pixmap = malloc(width*height*4*sizeof(uint8_t));
  uint8_t *pixel = pixmap;
  
  // Repeat each scanline for the height
  for (int scanline = 0; scanline < height; scanline ++) {
  
      // For each bar in the scanline
      for (int cur_bar = 0; cur_bar < 7; cur_bar++)  {

          // For each pixel in the bar
          for(int pix = 0; pix < bar_width; pix ++) {
              pixel[0] = bar_color[cur_bar*3+0];
              pixel[1] = bar_color[cur_bar*3+1];
              pixel[2] = bar_color[cur_bar*3+2];
              pixel[3] = 255;
              pixel += 4;
          }
      }
    print_pixmap8(pixmap, width, height);
    free(pixmap);
  }
}


