#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define col 255

// Method Declarations
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
  int pixmap_size = width*height*4;
  uint8_t *pixmap = malloc(pixmap_size);
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
    
    FILE *file = fopen("image.data", "w");
    fwrite(pixmap, 1, pixmap_size, file);
    fclose(file);
    
  }
}

