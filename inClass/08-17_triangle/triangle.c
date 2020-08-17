# include <stdio.h>
# include <stdlib.h>

int main( int argc, char *argv[]) {
   int charToInt = atoi(argv[1]);
   if ( argc == 2 ) {
      for( int base = 0; base <= charToInt; base ++) {
          for( int curRow = base; curRow > 0; curRow--) {
              printf("X");
          }
          printf("\n");
      }
      printf("\n");
    }
    else if( argc > 2 ) {
        printf("Too many arguments.\n");
    }
    else {
         printf("Enter the base of the triangle as an argument");
    }
}          
