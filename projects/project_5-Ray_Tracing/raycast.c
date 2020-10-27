#include "ppmrw.h"
#include "v3math.h"

int SPHERE = 999;
int PLANE = 998;
int CAMERA = 997;
int LIGHT = 996;

int numObjects = 0, numLights = 0;
float camWidth = 0, camHeight = 0;
int MAX_COLOR = 255;
int ns = 20;
float backgroundColor[3] = {0,0,0};

bool test = false;

typedef struct Light {
    float position[3], color[3];
    float radA[3];
    float direction[3];
    float cosTheta, angular;
    bool isSpotLight;

} Light;

typedef struct Object {

    int type; 
    float diffuse[3], specular[3];
    float position[3];
    float reflect;
    union {
        float radius;
        float normal[3];
    } value;

} Object;

Object objects[128];
Light lights[128];

// parser methods
void  checkValue(char str[], float *arr);
void  printObjects();
bool  resetValues (char *name, char *value);
void  readFile(char *filename);
int   setType(char str[]);
void  setValue(char name[], char value[], int type);

// ray cast methods
float getAngular(int lightNum, float *vObj);
float getRadial(int lightNum, float lightDistance);
void  illuminate(float *origin, float *direction, float *color, int objNum, int lightNum);
float intercect(float *origin, float *direction, int objOrigin, int *nearObj);
float planeIntersection(float *origin, float* direction, float* position, float* normal);
float sphereIntersection(float *origin, float* direction, float* position, float radius);
void  shoot(float *origin, float *direction, float *color, int recLevel);
 

//TODO: WHITE SPOTS
//TODO: TEST FILES
//TODO: SPOTLIGHT TEST
//TODO: REFLECT

///////////// CHECKVALUE /////////////////////////////////
void checkValue(char str[], float *arr) {
    int counter = 0;
    char value[100];
    value[0] = '\0';
    int chars;
    if (strchr(str, ',') != NULL) {
        for (chars = 0; chars < strlen(str) && counter < 3; chars++) { 
            if (isdigit(str[chars]) || str[chars] == '.' || str[chars] == '-') { 
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


/// TEMPORARY FUNCTION
///////////// PRINTOBJECTS /////////////////////////////////
void printObjects() {
    printf("Camera values:\n\tWidth = %f\n\tHeight = %f\n\n", camWidth, camHeight);
    printf("Total lights: %i\nTotal objects: %i\n", numLights + 1, numObjects + 1);
    for( int x = 0; x <= numLights; x++ ) {
        if(lights[x].isSpotLight) {
            printf("\nSpotLight Found:\n");
        }
        else {
            printf("\nLight Found:\n");
        }
        printf("\tposition = %f, %f, %f\n", lights[x].position[0], lights[x].position[1], lights[x].position[2]);
        printf("\tcolor = %f, %f, %f\n", lights[x].color[0], lights[x].color[1], lights[x].color[2]);
        printf("\tradial a0 = %f\n", lights[x].radA[0]);
        printf("\tradial a1 = %f\n", lights[x].radA[1]);
        printf("\tradial a2 = %f\n", lights[x].radA[2]);
        if(lights[x].isSpotLight) {
            printf("\tCos theta = %f\n", lights[x].cosTheta);
            printf("\tangular-a0 = %f\n", lights[x].angular);
            printf("\tdirection = %f, %f, %f\n", lights[x].direction[0], lights[x].direction[1], lights[x].direction[2]);
        }
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
        printf("\tdiffuse color = %f, %f, %f\n", objects[x].diffuse[0], objects[x].diffuse[1], objects[x].diffuse[2]);
        printf("\tspecular color = %f, %f, %f\n", objects[x].specular[0], objects[x].specular[1], objects[x].specular[2]);
        printf("\tposition = %f, %f, %f\n", objects[x].position[0], objects[x].position[1], objects[x].position[2]);
        printf("\treflectivity = %f\n", objects[x].reflect);
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
             else if (type != CAMERA && curChar != EOF && curChar != 'l') {
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
            setArray(lights[numLights].color, array); 
        }
        else if (strcmp(name, "position") == 0) {
            setArray(lights[numLights].position, array);
        }
        else if (strcmp(name, "theta") == 0) {
            if (atof(value) != 0) {
                lights[numLights].cosTheta = acosf(atof(value));
                lights[numLights].isSpotLight = true;
            }
        }
        else if (strcmp(name, "radial-a0") == 0) {
            lights[numLights].radA[0] = atof(value);
        }
        else if (strcmp(name, "radial-a1") == 0) {
            lights[numLights].radA[1] = atof(value);
        }
        else if (strcmp(name, "radial-a2") == 0) {
            lights[numLights].radA[2] = atof(value);
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
            setArray(objects[numObjects].diffuse, array);
        }    
        else if (strcmp(name, "specular_color") == 0) {
            setArray(objects[numObjects].specular, array);
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
        else if (strcmp(name, "reflectivity") == 0) {
            objects[numObjects].reflect = atof(value);
        }
        else {           
            fail("a value or name in an object is incorrect in the input file");
        }
    }
}

////////////////////////
///  RAYCAST METHODS  //
////////////////////////

///////////// GETANULAR /////////////////////////////////
float getAngular(int lightNum, float *vObj) {
    float angA = 1.0;
    if (lights[lightNum].isSpotLight) {
        angA = acos(v3_dot_product(lights[lightNum].direction, vObj));
        if (angA < lights[lightNum].cosTheta) {
            angA = 0;  
        }
        else {
	      angA = pow(angA, lights[lightNum].angular);
            //angA = pow(v3_dot_product(vObj, lights[lightNum].direction), lights[lightNum].angular);
        }
    }
    return angA;
}

///////////// GETRADIAL /////////////////////////////////
float getRadial(int lightNum, float lightDistance) {
    if (lightDistance == INFINITY) {
        return 1.0;
    }
    float denom = lights[lightNum].radA[0] + (lights[lightNum].radA[1] * lightDistance) + (lights[lightNum].radA[2] * sqr(lightDistance));
    assert(denom != 0); 
    return 1 / denom;

}
///////////// ILLUMINATION /////////////////////////////////
void illuminate(float *origin, float *direction, float *color, int objNum, int lightNum) {
   
    // Value L in equations
    float lightVect[3] = {0,0,0};
    v3_subtract(lightVect, lights[lightNum].position, origin);

    // set distance to light and normalize light vector
    float lightDistance = v3_length(lightVect);
    v3_normalize(lightVect, lightVect);
  
    // test if object inbetween current obj and light
    int nearObj = -1;
    test = true;
    float obstacle = intercect(origin, lightVect, objNum, &nearObj);

    // object in way, return shadow
    if (obstacle != INFINITY) {
        return;
    }

    // Value N in the equations
    float surfNorm[3] = {0,0,0};
    if (objects[objNum].type == PLANE) {
        setArray(surfNorm, objects[objNum].value.normal);              
    }
    else if (objects[objNum].type == SPHERE) {
        v3_subtract(surfNorm, origin, objects[objNum].position);  
    }  
    v3_normalize(surfNorm, surfNorm);

    // Value R in equations
    float reflectVect[3] = {0,0,0};
    v3_reflect(reflectVect, lightVect, surfNorm);

    // Value V in equations
    float viewVect[3] = {0,0,0};
    setArray(viewVect, direction);

    // get vObj
    float vObj[3] = {0,0,0};
    setArray(vObj, lightVect);
    v3_scale(vObj, -1);
    v3_normalize(vObj, vObj);

    // set attinuations
    float radA = getRadial(lightNum, lightDistance);
    float angA = getAngular(lightNum, vObj);

    // get diffuse
    float diffuse[3] = {0,0,0};
    v3_multiply(diffuse, objects[objNum].diffuse, lights[lightNum].color);
    v3_scale(diffuse, v3_dot_product(surfNorm, lightVect));
	//v3_normalize(diffuse, diffuse);

    // get specular
    float specular[3] = {0,0,0};
    v3_multiply(specular, objects[objNum].specular, lights[lightNum].color);
    v3_scale(specular, pow(v3_dot_product(reflectVect, viewVect), ns));
    //v3_scale(specular, pow(v3_dot_product(surfNorm, lightVect), ns));
	//v3_normalize(specular, specular);
    for (int x = 0; x < 3; x++) {
       float newColor = radA * angA * (specular[x] + diffuse[x]);
       if (newColor > 0 && newColor != INFINITY) {
           color[x] += newColor;
       }
    }
}

///////////// INTERCECT /////////////////////////////////
float intercect(float *origin, float *direction, int objOrigin, int *nearObj) {
    float closestDistance = INFINITY;
    float distance;
    for (int num = 0; num <= numObjects; num++) {
    //printf("\n Object Type: %d \n", objects[num].type);
   // TODO:
   // issue when the only object in the way is itself
   // if we skip the object the area that needs to be shaded will not have anything in its way and will have color
   // if we dont skip the object then it will have black spots, see Piazza question 
       if (num != objOrigin) {          
            if (objects[num].type == PLANE) {
                distance = planeIntersection(origin, direction, objects[num].position, objects[num].value.normal);
            }       
            else if (objects[num].type == SPHERE)  {
                distance = sphereIntersection(origin, direction, objects[num].position, objects[num].value.radius);
            }
            if (distance < closestDistance) {
                closestDistance = distance;
                *nearObj = num;   
            }
        }
        
    }
    return closestDistance;
}

///////////// PLANEINTERSECTION /////////////////////////////////
float planeIntersection(float *origin, float* direction, float* position, float* normal) {
    float distance = INFINITY;
    float numerator = -(v3_dot_product(normal, position));
    float denominator = v3_dot_product(normal, direction);
    if (denominator != 0) {
        distance = -(v3_dot_product(normal, origin) + numerator)/denominator;
    }
    if (distance <= 0) { 
        distance = INFINITY;
    }
    return distance;
}
///////////// INTERSECTION /////////////////////////////////
float sphereIntersection(float *origin, float* direction, float* position, float radius) {

    float distance = INFINITY;
    float diff[3] = {0,0,0};
    v3_subtract(diff, origin, position);

    float a = sqr(direction[0]) + sqr(direction[1]) + sqr(direction[2]);
    float b = 2 * v3_dot_product(direction, diff);
    float c = sqr(diff[0]) + sqr(diff[1]) + sqr(diff[2]) - sqr(radius);
    float discrim = sqr(b) - (4 * a * c);
    if (discrim >= 0) {
        float t0 = (-b - sqrtf(discrim)) / (2*a);
        float t1 = (-b + sqrtf(discrim)) / (2*a);
        
        if (t0 > 0) {
            distance = t0;
        }
        else if (t1 > 0) {
            distance = t1;
        }
    }
    return distance;
}

///////////// SHOOT /////////////////////////////////
void shoot(float *origin, float *direction, float *color, int recLevel) {    
    // finished recursing
    if (recLevel == 0) {
        return;
    }
    
    // get nearest object number and the distance
    int nearObj = -1;
    float distance = intercect(origin, direction, -1, &nearObj);

    // return if there is no nearest object
    if (distance == INFINITY) {  
        return;
    }

    float reflectColor[3] = {0,0,0};
    float objectColor[3] = {0,0,0};


    // set new origin at intercection
    float newOrigin[3] = {0,0,0};
    setArray(newOrigin, direction);
    v3_scale(newOrigin, distance);
    v3_add(newOrigin, newOrigin, origin);

    // trace ray if reflection 
    if (objects[nearObj].reflect > 0) {

        //TODO:  set new direction which is reflection point over the surface normal
		float surfNorm[3] = {0,0,0};
        float newDirection[3] = {0,0,0};
        v3_subtract(surfNorm, newOrigin, objects[nearObj].position);
        v3_normalize(surfNorm,surfNorm);
        v3_reflect(newDirection, direction, surfNorm);

        shoot(newOrigin, newDirection, reflectColor, recLevel-1); //recurse

    }

    assert(objects[nearObj].reflect <= 1);

    float opacity = 1 - objects[nearObj].reflect;

    if (opacity > 0) { 
        for (int lightNum = 0; lightNum <= numLights; lightNum++) {
            illuminate(newOrigin, direction, objectColor, nearObj, lightNum);
			//printf("\nILLUMINATE!\n");
        }
    }

    for (int x = 0; x < 3; x++) {
        color[x] = opacity * objectColor[x] + reflectColor[x] * objects[nearObj].reflect;
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

    float pixHeight = camHeight/image->height;
    float pixWidth = camWidth/image->width;
    float finalColor[3] = {0,0,0};

    for(int row = 0; row < image->width; row++) {

        for(int col = 0; col < image->height; col++) {
            float origin[3] = {0,0,0};
            float direction[3] = {origin[0] - (camHeight / 2) + (pixHeight * (row+0.5)),
                                  -(origin[1] - (camWidth  / 2) + (pixWidth *  (col+0.5))),
                                  -1};

            v3_normalize(direction, direction); 
            setArray(finalColor, backgroundColor);
            
            shoot(origin, direction, finalColor, 7); 

            int location = col*image->width*3+row*3;
            for (int x = 0; x < 3; x++) { // set color to pixMap
                image->pixData[location + x] = clamp(finalColor[x] * MAX_COLOR, MAX_COLOR);
            }
        }
    }
    writeP3(image, output);
    printf("\nCreated image %s\n\n", output);
    free(image);
    return(0);
}
