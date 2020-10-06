#include "ppmrw.h"

typedef struct Camera {
    float width;
    float height;
} Camera;

typedef struct Sphere {
    float color;
    float poisiton;
    float radius;
} Sphere;

typedef struct Plane {
    float color;
    float position;
    float normal;
} Plane;


void readLine(char* filename) {
     FILE *file = fopen(filename, "r");
     char curChar = getc(file);

     while (curChar != EOF) {
         char line[] = "";
         while (curChar != '\n' && curChar != EOF) {
             strncat(line, &curChar, 1);
             curChar = getc(file);
         }
         char* token = strtok(line, ",");
         while (token != NULL) {
             printf( " %s", token);
             token = strtok(NULL, ",");
         }
         curChar = getc(file);
         printf("\n");
     } 
     fclose(file);      
}

///////////// SHOOT /////////////////////////////////
// I believe this is a function we need according to section 4.1 of House's Raycasting
//shoot() is a function that “shoots” the ray out into the scene,
//and returns the position of the first intersection of the ray with an object in the
//scene

///////////// SHADE /////////////////////////////////
// I believe this is a function we need according to section 4.1 of House's Raycasting
//shade() function determines what color to assign the current pixel,
//based on the position of the “hit” returned by shoot()


/////////////   MAIN  ///////////////////////////////
int main (int argc, char *argv[]) {

    // number of arguments is incorrect
    if (argc != 5) {
       fail("Invaild number of arguments");
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    char *input = argv[3];
    char *output = argv[4];

    // First argument given is invalid
    if (width <= 0) {
        fail( "Invalid Width argument");
    }
    // Second argument given is invalid
    else if (height <= 0) {
        fail( "Invalid Height argument");
    }
    // Third Argument: file does not exist
    else if (!fileExists(input)) {
        fail("input file does not exist");
    }
    // Third Argument: incorrect type
    else if(!isFileType(input, ".json")) {
        fail("input file is not of type .json");
    }
    // Fourth Argument: incorrect type
    else if(!isFileType(output, ".ppm")) {
        fail("output file is not of type .ppm");
    }
    
    readLine(input);
    
    return(0);
}
