#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Method Declarations
void fail(char* s);
void change_color(int cur_bar, uint8_t *pixmap);
int main(int argc, char *argv[]);

// fail case
void fail(char* s) {
  printf("Error: %s\n\n", s);
  printf("Usage:\n");
  printf("test_pattern WIDTH HEIGHT\n\n");
  exit(1);
}

// change_color
void change_color(int cur_bar, uint8_t *pixmap) {
   switch(cur_bar) {

     // white
      case 0:
          pixmap[0] = 255; // Red
          pixmap[1] = 255; // Green
          pixmap[2] = 255; // Blue
          pixmap[3] = 255; // Alpha
          break;

      // yellow
      case 1:
          pixmap[0] = 255; // Red
          pixmap[1] = 255; // Green
          pixmap[2] = 0;   // Blue
          pixmap[3] = 255; // Alpha
          break;

      // cyan
      case 2:
          pixmap[0] = 0;   // Red
          pixmap[1] = 255; // Green
          pixmap[2] = 255; // Blue
          pixmap[3] = 255; // Alpha
          break;

      // green
      case 3:
          pixmap[0] = 0;   // Red
          pixmap[1] = 255; // Green
          pixmap[2] = 0;   // Blue
          pixmap[3] = 255; // Alpha
          break;

      // magenta
      case 4:
          pixmap[0] = 255; // Red
          pixmap[1] = 0;   // Green
          pixmap[2] = 255; // Blue
          pixmap[3] = 255; // Alpha
          break;

      // red
      case 5:
          pixmap[0] = 255; // Red
          pixmap[1] = 0;   // Green
          pixmap[2] = 0;   // Blue
          pixmap[3] = 255; // Alpha
          break;

      // blue
      case 6:
          pixmap[0] = 0;   // Red
          pixmap[1] = 0;   // Green
          pixmap[2] = 255; // Blue
          pixmap[3] = 255; // Alpha
          break;
   }
}

// main function
int main(int argc, char *argv[]) {

  // variable declarations
  int bar_width, width, height;
  uint8_t *pixmap;
  
  //  incorrect amount of input by user
  if (argc != 3) {
    fail("Wrong number of arguments.");
  }

  bar_width = atoi(argv[1]);
  width = bar_width * 7;
  height = atoi(argv[2]);

  // invalid input by user
  if(width <= 0 || height <= 0){
    fail("Height or Width non-positive integer");
  }

  // create pixmap
  pixmap = malloc(width*height*4*sizeof(uint8_t));

  // Repeat each scanline for the height
  for (int scanline = 0; scanline < height; scanline ++) {
  
      // For each bar in the scanline
      for (int cur_bar = 0; cur_bar < 7; cur_bar++)  {
          change_color(cur_bar, pixmap);

          // For each pixel in the bar
          for(int pixel = 0; pixel < bar_width; pixel ++) {
              printf("[ %d, %d, %d, %d ]", pixmap[0],pixmap[1],pixmap[2],pixmap[3]);
          }
      }
     printf("\n");
  }
}


