#include "ppmrw.h"
#include "v3math.h"

int SPHERE = 999;
int PLANE = 998;
int CAMERA = 997;
int LIGHT = 996;

int numObjects = 0, numLights = 0;
float camWidth = 0, camHeight = 0;
int MAX_COLOR = 1;

float origin[3] = {0,0,0};
float backgroundColor[3] = {0,0,0};

typedef struct Light {
    float position[3], color[3];
    float radA0, radA1, radA2;
    float direction[3];
    float theta, angular;

} Light;

typedef struct Object {

    int type; 
    float diffColor[3], specColor[3];
    float position[3];
    union {
        float radius;
        float normal[3];
    } value;

} Object;

Object objects[128];
Light lights[128];

// methods
void checkValue(char str[], float *arr);
void getDircetion(float *rayDirection, float x, float y, float z);
void illuminate (float *color, float x, float y, float distance);
float intersection(float *direct, int objNum);
void printObjects();
void raycast(PPM *image);
bool resetValues (char *name, char *value);
void readFile(char *filename);
void setColor( float *array, float *color_vals);
int setType(char str[]);
void setValue(char name[], char value[], int type);


///////////// CHECKVALUE /////////////////////////////////
void checkValue(char str[], float *arr) {
    int counter = 0;
    char value[100];
    value[0] = '\0';
    int chars;
    if (strchr(str, ',') != NULL) {
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
}

///////////// ILLUMINATE /////////////////////////////////
//TODO:
void illuminate (float *color, float x, float y, float distance) {
    for(int num = 0; num < numLights; num++) {

    }

}


///////////// INTERSECTION /////////////////////////////////
float intersection(float *direct, int objNum) {
    float distance = INFINITY;
    float temp[3] = {0,0,0};

    v3_subtract(temp, origin, objects[objNum].position);

    if (objects[objNum].type == SPHERE) {

        float b = 2 * v3_dot_product(direct, temp);
        float c = sqr(temp[0]) + sqr(temp[1]) + sqr(temp[2]) - sqr(objects[objNum].value.radius);
        float discrim = sqr(b) - (4 * c);

        if (discrim > 0) {
            float t0 = (b - sqrtf(discrim))/2;
            float t1 = (b + sqrtf(discrim))/2;
            if (t0 > 0 && t0 < t1) {
                distance = t0;
            }
            else if (t1 > 0) {
                distance = t1;
            }

        }
    }
    else if (objects[objNum].type == PLANE) {

        float numerator = v3_dot_product(temp, objects[objNum].value.normal);
        float denominator = v3_dot_product(direct, objects[objNum].value.normal);

        if (numerator != 0 && denominator != 0 && -1 * numerator/denominator > 0) {
            distance = -1*numerator/denominator;
        }
    }
    return distance;
}


/// TEMPORARY FUNCTION
///////////// PRINTOBJECTS /////////////////////////////////
void printObjects() {
    printf("Camera values:\n\tWidth = %f\n\tHeight = %f\n", camWidth, camHeight);
    for( int x = 0; x <= numLights; x++ ) {
        printf("\nLight Found:\n");
        printf("\tposition = %f, %f, %f\n", lights[x].position[0], lights[x].position[1], lights[x].position[2]);
        printf("\tcolor = %f, %f, %f\n", lights[x].color[0], lights[x].color[1], lights[x].color[2]);
        printf("\tradial a0 = %f\n", lights[x].radA0);
        printf("\t\ta1 = %f\n", lights[x].radA1);
        printf("\t\ta2 = %f\n", lights[x].radA2);
        printf("\ttheta = %f\n", lights[x].theta);
        printf("\tangular-a0 = %f\n", lights[x].angular);
        printf("\tdirection = %f, %f, %f\n", lights[x].direction[0], lights[x].direction[1], lights[x].direction[2]);
    }
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
        printf("\tdiffuse color = %f, %f, %f\n", objects[x].diffColor[0], objects[x].diffColor[1], objects[x].diffColor[2]);
        printf("\tspecular color = %f, %f, %f\n", objects[x].specColor[0], objects[x].specColor[1], objects[x].specColor[2]);
        printf("\tposition = %f, %f, %f\n", objects[x].position[0], objects[x].position[1], objects[x].position[2]);
    }
} 

///////////// RAYCAST /////////////////////////////
void raycast(PPM *image){
    float pixHeight = camHeight/image->height;
    float pixWidth = camWidth/image->width;
    float finalColor[3] = {0,0,0};

    for(int row = 0; row < image->height; row++) {

        for(int col = 0; col < image->width; col++) {

            float direct[3] = {origin[0] - (camHeight / 2) + (pixHeight * (row+0.5)),
                               origin[1] - (camWidth / 2) + (pixWidth *  (col+0.5)),
                               -1};

            v3_normalize(direct, direct);

            int location = (col*image->width*3)+(row*3);
            int nearObj = 0;
            float nearDist = INFINITY;

            setArray(finalColor, backgroundColor);

            for (int num = 0; num <= numObjects; num++) {
                float intersectDist = intersection(direct, num);          
                if (intersectDist < nearDist) {
                    nearDist = intersectDist;
                    nearObj = num;
                }
            }
            if (nearDist != INFINITY) { // set color to finalColor
                setArray(finalColor, objects[nearObj].diffColor);
                illuminate(finalColor, row, col, nearDist); 
            }
            for (int x = 0; x < 3; x++) { // set color to pixMap
                image->pixData[location + x] = finalColor[x];
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
     char curChar = tolower(getc(file));
     char name[100], value[100];
     bool isName = resetValues(name, value), inTuple = false;
     int type = 0;
     while (curChar != EOF) {
         if (curChar == '\n') {
             setValue(name, value, type);             
             curChar = tolower(getc(file));
             if (type == LIGHT && curChar != EOF) {
                 numLights++;
             }
             else if (type != CAMERA && curChar != EOF) {
                 numObjects++;
             }
             curChar = tolower(ungetc(curChar, file));
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
                 type = setType(name);
             }
             else {
                 setValue(name, value, type);
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
     fclose(file);      
} 

///////////// SETCOLOR /////////////////////////////////
void setColor( float* array, float *color_vals) {
    for(int x = 0; x < 3; x++) {
        if(array[x] > MAX_COLOR) {
            array[x] = MAX_COLOR;
        }
        else if(array[x] < 0) {
            array[x] = 0;
        }
        else {
            array[x] = color_vals[x];
       }
    }
}

///////////// SETTYPE /////////////////////////////////
int setType(char str[]) {
    if (strcmp(str, "camera") == 0) {
        return CAMERA;
    }
    else if (strcmp(str, "light") == 0) {
        return LIGHT;
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

///////////// SETVALUE /////////////////////////////////
void setValue(char name[], char value[], int type) {

    float array[3] = {0,0,0};
    checkValue(value, array);

    // Set Camera value
    if (type == CAMERA) {
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
    // Set Light value
    else if (type == LIGHT) {
        if (isType(name, "color")) {
            setColor(lights[numLights].color, array); 
        }
        else if (strcmp(name, "position") == 0) {
            setArray(lights[numLights].position, array);
        }
        else if (strcmp(name, "theta") == 0) {
            lights[numLights].theta = atof(value);
        }
        else if (strcmp(name, "radial-a0") == 0) {
            lights[numLights].radA0 = atof(value);
        }
        else if (strcmp(name, "radial-a1") == 0) {
            lights[numLights].radA1 = atof(value);
        }
        else if (strcmp(name, "radial-a2") == 0) {
            lights[numLights].radA2 = atof(value);
        }
        else if (strcmp(name, "angular-a0") == 0) {
            lights[numLights].angular = atof(value);
        }
        else if (strcmp(name, "direction") == 0) {
            setArray(lights[numLights].direction, array);
        }
        else {
            fail("a value or name in an object is incorrect in the input file");
        }
    }
    // Set Object value
    else {
        if (strcmp(name, "diffuse_color") == 0) {
            setColor(objects[numObjects].diffColor, array);
        }    
        else if (strcmp(name, "specular_color") == 0) {
            setColor(objects[numObjects].specColor, array);
        }
        else if (strcmp(name, "position") == 0) {
            setArray(objects[numObjects].position, array);
        }
        else if (strcmp(name, "radius") == 0 && atof(value) != 0) {
            objects[numObjects].value.radius = atof(value);
        }
        else if (strcmp(name, "normal") == 0) {
            setArray(objects[numObjects].value.normal, array);
        }
        else {           
            fail("a value or name in an object is incorrect in the input file");
        }
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
    else if(!isType(input, ".json") && !isType(input, ".csv")){
        fail("input file is not of type .json");
    }
    // Fourth Argument: incorrect type
    else if(!isType(output, ".ppm")) {
        fail("output file is not of type .ppm");
    }
    
    readFile(input);
   
    if (camHeight == 0 || camWidth == 0 ) {
        fail("camera not properly set");
    }
    
    printObjects(); //remove later

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

