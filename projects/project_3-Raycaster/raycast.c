#include "ppmrw.h"
#include "v3math.h"

int SPHERE = 999;
int PLANE = 998;
int CAMERA = 997;

int numObjects = 0;
float camWidth = 0, camHeight = 0;
int MAX_COLOR = 1;

float origin[3] = {0,0,0};
float backgroundColor[3] = {0,0,0};
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

// methods
void getTuple(char str[], float *arr);
void getDircetion(float *rayDirection, float x, float y, float z);
float intersection(float *direct, int objNum);
void printObjects();
void raycast(PPM *image);
bool resetValues (char* name, char* value);
void readFile(char* filename);
void setArray(float *dest, float *arr);
void setCamVal(char name[], char value[]);
int setObjName(char str[]);
void setObjVal(char name[], char value[]);


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

///////////// INTERSECTION /////////////////////////////////
float intersection(float *direct, int objNum) {
    float distance = INFINITY, value;
    float temp[3] = {0,0,0};

    v3_subtract(temp, origin, objects[objNum].center);

    if (objects[objNum].type == SPHERE) {

        float b = 2 * v3_dot_product(direct, temp);
        float c = sqr(temp[0]) + sqr(temp[1]) + sqr(temp[2]) - sqr(objects[objNum].value.radius);
        float discrim = sqr(b) - (4 * c);

        if (discrim > 0) {
            value = (b + sqrtf(discrim))/2;
            if (value > 0) {
               distance = value;
            }
        }
    }
    else if (objects[objNum].type == PLANE) {

        v3_subtract(temp, origin, objects[objNum].center);

        value = v3_dot_product(temp, objects[objNum].value.normal);
        float denominator = v3_dot_product(direct, objects[objNum].value.normal);

        if (value != 0 && denominator != 0 && -1 * value/denominator > 0) {
            distance = -1*value/denominator;
        }
    }
    return distance;
}


/// TEMPORARY FUNCTION
///////////// PRINTOBJECTS /////////////////////////////////
void printObjects() {
   printf("Camera values:\n\tWidth = %f\n\tHeight = %f\n", camWidth, camHeight);
   for( int x = 0; x <= numObjects; x++ ) {
      if( objects[x].type == SPHERE) {
         printf("\nObject %d: Sphere\n", x + 1);
         printf("\tRadius = %f\n", objects[x].value.radius);
      }
      else if( objects[x].type == PLANE) {
         printf("\nObject %d: Plane\n", x + 1);
         printf("\tnormal = %f, %f, %f\n", objects[x].value.normal[0], 
                                           objects[x].value.normal[1], 
                                           objects[x].value.normal[2]);
      }
      printf("\tcolor = %f, %f, %f\n", objects[x].color[0], objects[x].color[1], objects[x].color[2]);
      printf("\tcenter = %f, %f, %f\n", objects[x].center[0], objects[x].center[1], objects[x].center[2]);
   }
} 

///////////// RAYCAST /////////////////////////////
void raycast(PPM *image){
    float xdist, ydist, zdist = -1;

    float pixHeight = camHeight/image->height, pixWidth = camWidth/image->width;

    for(int row = 0; row < image->height; row++){

        xdist = origin[0] - (camHeight / 2) + (pixHeight * (row+0.5));

        for(int col = 0; col < image->width; col++){

            ydist = origin[1] - (camWidth / 2) + (pixWidth *  (col+0.5));

            float direct[3] = {xdist-origin[0], ydist-origin[1], zdist-origin[2]};
            v3_normalize(direct, direct);

            int nearObj = 0;
            float nearDist = INFINITY;
            for (int num = 0; num <= numObjects; num++) {
                float intersectDist = intersection(direct, num);          
                if (intersectDist < nearDist) {
                    nearDist = intersectDist;
                    nearObj = num;
                }
            }
            int location = (col*image->width*3)+(row*3);
            if (nearDist != INFINITY) {
                image->pixData[location] = objects[nearObj].color[0];
                image->pixData[location+1] = objects[nearObj].color[1];
                image->pixData[location+2] = objects[nearObj].color[2];
            }
            else {
                image->pixData[location] = backgroundColor[0];
                image->pixData[location+1] = backgroundColor[1];
                image->pixData[location+2] = backgroundColor[2];
            }
        }
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
     FILE *file = fopen(filename, "r");
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
    if (strcmp(name, "width")) {
        camWidth = atof(value);
    }
    else if (strcmp(name, "height")) {
        camHeight = atof(value);
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
        for(int x = 0; x < 3; x++) {
            if(array[x] <= MAX_COLOR && array[x] >= 0) {
                objects[numObjects].color[x] = array[x];
            }
            else {
                failure = true;
            }
        }
    }
    else if (strcmp(name, "position") == 0) {
        getTuple(value, array);
        setArray(objects[numObjects].center, array);
    }
    else if (strcmp(name, "radius") == 0) {
        if (atof(value) == 0) {
            failure = true;
        }
        objects[numObjects].value.radius = atof(value);
    }
    else if (strcmp(name, "normal") == 0) {
        getTuple(value, array);
        setArray(objects[numObjects].value.normal, array);
    }
    else {
        failure = true;
    }
    if (failure) {
        fail("a value or name in an object is incorrect in the input file");
    }

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
    else if(!isFileType(input, ".json") && !isFileType(input, ".csv")){
        fail("input file is not of type .json");
    }
    // Fourth Argument: incorrect type
    else if(!isFileType(output, ".ppm")) {
        fail("output file is not of type .ppm");
    }
    
    readFile(input);
   
    if (camHeight == 0 || camWidth == 0 ) {
        fail("camera not properly set");
    }
    
    //printObjects(); //remove later

    // create image
    PPM *image = (PPM*)malloc(sizeof(PPM));
    image->pixData = (unsigned char*)malloc(width*height*sizeof(Pixel)); 
    image->maxColVal = MAX_COLOR;  
    image->width = width;
    image->height = height;

    raycast(image);
    writeP3(image, output);
    printf("\nCreated image %s\n\n", output);
    free(image);
    return(0);
}

