#include "ppmrw.h"

float camWidth = .5;
float camHeight = .5;

float BLACK[3] = {0,0,0};

typedef struct object_t {

    int type; // 998 Plane; 999 sphere
    float color[3];
    float center[3];

   union{
      float radius;
      float normal[3];
   } value;

};

struct object_t objects[128];
int objectCount = 0;


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
///////////// GETPIXDATA /////////////////////////////
void getPixData (float input[], float **output, float imgHeight, float imgWidth){

   int rowIndex = 0, colIndex = 0;
   float pixY,pixX, pixZ;
   float unitRayVector[3];
   float ray[3];

   float pixHeight = camHeight/imgHeight;
   float pixWidth = camWidth/imgWidth;
   float xImgCenter = imgWidth/2;
   float yImgCenter = imgHeight/2;



   for(rowIndex; rowIndex < pixHeight; rowIndex++){
      //py = cy - h/2 + pixHeight * (rowIndex + .5) // Y COORD OF ROW
      pixY = yImgCenter - (camHeight/2) + (pixHeight * (rowIndex + .5));
      for(colIndex; colIndex < pixWidth; colIndex++){
         //px = cx - w/2 + pixwidth * (colIndex + .5) // X COORD OF COLUMN
         pixX = xImgCenter - (camWidth/2) + (pixWidth * (colIndex + .5));
         //pz = -zp; // Z COORD IS ON SCREEN ( THE PLANE IS -1 )
         pixZ = -1;
         //ur = p/||P|| //UNIT RAY VECTOR (P divided by P magnitude (length))
         //unitRayVector = p/(length(p))
         unitRayVector[0] = input[0]/(length(input[0]));
         unitRayVector[1] = input[1]/(length(input[1]));
         unitRayVector[2] = input[2]/(length(input[2]));
         //x = shoot(unitRayVector) // RETURN POSITION ON FIRST HIT
         shoot(&unitRayVector);
         memcpy(ray,unitRayVector, sizeof(ray));
         if(ray!= NULL){
            //image[rowIndex][colIndex] = shade(x); // PIXEL COLORED BY OBJECT HIT
            output[rowIndex][colIndex] = shade(ray);
         }
         else{
            output[rowIndex][colIndex] = shade(BLACK);
         }

      }
   }
}
///////////// SHOOT /////////////////////////////////
// I believe this is a function we need according to section 4.1 of House's Raycasting
//shoot() is a function that “shoots” the ray out into the scene,
//and returns the position of the first intersection of the ray with an object in the
//scene

void shoot(float *unitRayVector){

   float foundObject[3];
   float foundObjectColor[3];
   int objIndex;

   for(objIndex = 0; objIndex < objectCount; objIndex++){
      if (objects[objIndex].center[0] == unitRayVector[0] &&
          objects[objIndex].center[1] == unitRayVector[1] &&
          objects[objIndex].center[2] == unitRayVector[2]){
             memcpy(unitRayVector, foundObject, sizeof(unitRayVector));
             unitRayVector[0] = objects[objIndex].color[0];
             unitRayVector[1] = objects[objIndex].color[1];
             unitRayVector[2] = objects[objIndex].color[2];
      }
   }
}

///////////// SHADE /////////////////////////////////
// I believe this is a function we need according to section 4.1 of House's Raycasting
//shade() function determines what color to assign the current pixel,
//based on the position of the “hit” returned by shoot()
void shade(float foundColors[], float imgX, float imgY){



}


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
