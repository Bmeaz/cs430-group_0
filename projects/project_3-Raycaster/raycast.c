#include "ppmrw.h"
#include "v3math.h"

int SPHERE = 999;
int PLANE = 998;
int CAMERA = 997;

int numObjects = 0;
int camWidth = 0, camHeight = 0;


float BLACK[3] = {0,0,0};

typedef struct Object {

    int type; 
    float color[3];
    float center[3];
    union {
        float radius;
        float normal[3];
    } value;

} Object;

struct Object objects[128];

void getTuple(char str[], float *arr);
void printObjects();
bool resetValues (char* name, char* value);
void readFile(char* filename);
void setCamVal(char name[], char value[]);
int setObjName(char str[]);
void setObjVal(char name[], char value[]);
 /*
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
*/
///////////// GETTUPLE /////////////////////////////////
void getTuple(char str[], float *arr) {
    int counter = 0;
    char value[100];
    value[0] = '\0';
    int chars;
    for (chars = 0; chars < strlen(str) && counter < 3; chars++) {
        
        if (isdigit(str[chars]) || str[chars] == '.') { 
            strncat(value, &str[chars], 1);
        }
        else if (str[chars] == ',') {
            arr[counter] = atof(value);
            counter ++;
            value[0] = '\0';
        }
    }
    if (counter != 2 && value[0] != '\0') {
        fail("Incorrect number of value in tuple for object");
    }
    else if (chars == strlen(str)) {
        arr[counter] = atof(value);
    }
}

/// TEMPORARY FUNCTION
///////////// PRINTOBJECTS /////////////////////////////////
void printObjects() {
   for( int x = 0; x <= numObjects; x++ ) {
      if( objects[x].type == SPHERE) {
         printf("\nObject found: Sphere\n");
         printf("\tRadius = %f\n", objects[x].value.radius);
      }
      else if( objects[x].type == PLANE) {
         printf("\nObject found: Plane\n");
         printf("\tnormal = %f, %f, %f\n", objects[x].value.normal[0], 
                                           objects[x].value.normal[1], 
                                           objects[x].value.normal[2]);
      }
      printf("\tcolor = %f, %f, %f\n", objects[x].color[0], objects[x].color[1], objects[x].color[2]);
      printf("\tcenter = %f, %f, %f\n", objects[x].center[0], objects[x].center[1], objects[x].center[2]);
   }
} 
  
///////////// RESETVALUES /////////////////////////////////
bool resetValues (char* name, char* value) {
    name[0] = '\0';
    value[0] = '\0';    
    return true;           
}   
 
///////////// READFILE /////////////////////////////////
void readFile(char* filename) {
     FILE *file = fopen(filename, "w");
     char curChar = getc(file);
     char name[100], value[100];
     bool isName = resetValues(name, value), inTuple = false;
     int type = 0;
     while (curChar != EOF) {
         if (curChar == '\n') {
             if (type != CAMERA) {  
                 setObjVal(name, value); 
                 numObjects++;
             }
             else {
                 setCamVal(name, value);
             }
             type = 0;
             isName = resetValues(name, value);
         }
         else if (curChar == '[') {
             inTuple = true;
         }
         else if (curChar == ']') {
             inTuple = false;
         }
         else if (curChar == ',' && !inTuple) {
             if (type == 0) {
                 type = setObjName(name);
             }
             else if (type == CAMERA) {
               setCamVal(name, value);
             }
             else {
               setObjVal(name, value);
             }
             isName = resetValues(name, value);
         }
         else if (curChar == ':') {
             isName = false; 
         }
         else if (isName && curChar != ' ') {
              strncat(name, &curChar, 1);
         }
         else if (curChar != ' ') {
              strncat(value, &curChar, 1);             
         }
         curChar = tolower(getc(file));
     }
     numObjects--;
     fclose(file);      
}


   
///////////// SETCAMVALUE /////////////////////////////////
void setCamVal(char name[], char value[]) {
    float val = atof(value);
    if (strcmp(name, "width")) {
        camWidth = val;
    }
    else if (strcmp(name, "height")) {
        camHeight = val;
    }
    else {
        fail("a value or name of the camera is incorrect in the input file");
    }
}

///////////// SETOBJNAME /////////////////////////////////
int setObjName(char str[]) {
    
    if (strcmp(str, "camera") == 0) {
        return CAMERA;
    }
    else if (strcmp(str, "sphere") == 0) {
        objects[numObjects].type = SPHERE;
        return SPHERE;
    }
    else if (strcmp(str, "plane") == 0) {
        objects[numObjects].type = PLANE;
        return PLANE;
    }
    fail("invaild object in file");
    return 0;
}

///////////// SETOBJVAL /////////////////////////////////
void setObjVal(char name[], char value[]) {
    float array[3] = {0,0,0};
    bool failure = false;
    if (strcmp(name, "color") == 0) {
        getTuple(value, array);
        objects[numObjects].color[0] = array[0];
        objects[numObjects].color[1] = array[1];
        objects[numObjects].color[2] = array[2];     
    }
    else if (strcmp(name, "position") == 0) {
        getTuple(value, array);
        objects[numObjects].center[0] = array[0];
        objects[numObjects].center[1] = array[1];
        objects[numObjects].center[2] = array[2];
    }
    else if (strcmp(name, "radius") == 0) {
        if (atof(value) == 0) {
            failure = true;
        }
        objects[numObjects].value.radius = atof(value);
    }
    else if (strcmp(name, "normal") == 0) {
        getTuple(value, array);
        objects[numObjects].value.normal[0] = array[0];
        objects[numObjects].value.normal[1] = array[1];
        objects[numObjects].value.normal[2] = array[2];
    }
    else {
        failure = true;
    }
    if (failure) {
        fail("a value or name in an object is incorrect in the input file");
    }

}
/*
///////////// SHADE /////////////////////////////////
// I believe this is a function we need according to section 4.1 of House's Raycasting
//shade() function determines what color to assign the current pixel,
//based on the position of the “hit” returned by shoot()
void shade(float foundColors[], float imgX, float imgY){



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
*/
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
    else if(!isFileType(input, ".json") && !isFileType(input, ".csv")){
        fail("input file is not of type .json");
    }
    // Fourth Argument: incorrect type
    else if(!isFileType(output, ".ppm")) {
        fail("output file is not of type .ppm");
    }
    
    readFile(input);
    printObjects();
    return(0);
}
