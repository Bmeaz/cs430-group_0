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

typedef struct Light {
    float position[3], color[3];
    float radA[3];
    float direction[3];
    float cosTheta, angular;
    bool isSpotLight;

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
void  checkValue(char str[], float *arr);
void illuminate(float *origin, float *direction, float *color, int objNum, int lightNum);
float planeIntersection(float *origin, float* direction, float* center, float* normal);
void  printObjects();
bool  resetValues (char *name, char *value);
void  readFile(char *filename);
int   setType(char str[]);
void  setValue(char name[], char value[], int type);
float sphereIntersection(float *origin, float* direction, float* center, float radius);
void  shoot(float *origin, float *direction, float *color, int recLevel);
 

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

///////////// ILLUMINATION /////////////////////////////////
void illuminate(float *origin, float *direction, float *color, int objNum, int lightNum) {
   
    // set radial attinuation
    float lightDistance = sqrtf(sqr(direction[0]) + sqr(direction[1]) + sqr(direction[2]));
    float denom = lights[lightNum].radA[0] + (lights[lightNum].radA[1] * lightDistance) + (lights[lightNum].radA[2] * sqr(lightDistance));
    assert(denom != 0); 
    float radA = 1 / denom;

    // set angular attinuation
    float angA = 1.0;
    if (lights[lightNum].isSpotLight) {
        angA = pow(v3_dot_product(lights[lightNum].position, direction), lights[lightNum].angular);
        if (angA < lights[lightNum].cosTheta) {
            angA = 0;  
        }
    }

    float objDistance = lightDistance;
    float currentDistance;
    // loop through all objects
    for (int curObj = 0; curObj <= numObjects; curObj++) {
        if (curObj != objNum) {       
            if (objects[curObj].type == PLANE) {
                currentDistance = planeIntersection(origin, direction, objects[curObj].position, objects[curObj].value.normal);             
            }       
            else {
                currentDistance = sphereIntersection(origin, direction, objects[curObj].position, objects[curObj].value.radius);
            }
            if (currentDistance < objDistance && currentDistance > 0) {
                objDistance = currentDistance;
            }
            if (fabs(objDistance - lightDistance) < 0.00000001) {

                // Value N in the equations
                float surfNorm[3] = {0,0,0};
                if (objects[objNum].type == PLANE) {
                    setArray(surfNorm, objects[objNum].value.normal);              
                }
                else if (objects[objNum].type == SPHERE) {
                    v3_subtract(surfNorm, origin, objects[objNum].position);  
                }  
                v3_normalize(surfNorm, surfNorm);

                // Value L in equations
                float lightVect[3] = {0,0,0};
                setArray(lightVect, direction);

                // Value R in equations
                float reflectVect[3] = {0,0,0};
                for (int a = 0; a < 3; a++) {
                    reflectVect[a] = direction[a] - (2  * v3_dot_product(surfNorm, direction) * surfNorm[a]);
                }

                // Value V in equations
                float viewVect[3] = {0,0,0};
                setArray(viewVect, direction);
                v3_scale(viewVect, -1);
                for (int x = 0; x < 3; x++) {
                    // the following two variables should not be the absolute(fabs) but for some reason it works when it is
                    float diffDotProd = fabs(surfNorm[x] * lightVect[x]);
                    float specDotProd = fabs(reflectVect[x] * viewVect[x]);
                    if (diffDotProd != 0 && specDotProd != 0) {
                            float diffuse = objects[objNum].diffColor[x] * lights[lightNum].color[x] * diffDotProd;
                            float specular = objects[objNum].specColor[x] * lights[lightNum].color[x] * pow(specDotProd, ns);
                            color[x] += radA * angA * (diffuse + specular);
                    }
                } 
            }
        }
    }   
}

///////////// PLANEINTERSECTION /////////////////////////////////
float planeIntersection(float *origin, float* direction, float* center, float* normal) {
    float numerator = -(v3_dot_product(normal, center));
    float denominator = v3_dot_product(normal, direction);
    if (numerator != 0 && denominator != 0 && -1 * numerator/denominator > 0) {
        return -(v3_dot_product(normal, origin) + numerator)/denominator;
    }
    return INFINITY;
}


/// TEMPORARY FUNCTION
///////////// PRINTOBJECTS /////////////////////////////////
void printObjects() {
    printf("Camera values:\n\tWidth = %f\n\tHeight = %f\n", camWidth, camHeight);
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
        printf("\t\ta1 = %f\n", lights[x].radA[1]);
        printf("\t\ta2 = %f\n", lights[x].radA[2]);
        printf("\tCos theta = %f\n", lights[x].cosTheta);
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
                 if (lights[numLights].angular != 0) {
                    lights[numLights].isSpotLight = true;
                 }
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
            lights[numLights].cosTheta = acosf(atof(value));
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
            setArray(objects[numObjects].diffColor, array);
        }    
        else if (strcmp(name, "specular_color") == 0) {
            setArray(objects[numObjects].specColor, array);
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

///////////// INTERSECTION /////////////////////////////////
float sphereIntersection(float *origin, float* direction, float* center, float radius) {

    float distance = INFINITY;
    float temp[3] = {0,0,0};
    v3_subtract(temp, origin, center);

    float a = sqr(direction[0]) + sqr(direction[1]) + sqr(direction[2]);
    float b = 2 * v3_dot_product(direction, temp);
    float c = sqr(temp[0]) + sqr(temp[1]) + sqr(temp[2]) - sqr(radius);
    float discrim = sqr(b) - (4 * a * c);

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
    return distance;
}

///////////// SHOOT /////////////////////////////////
void shoot(float *origin, float *direction, float *color, int recLevel) {    

    if (recLevel == 0) {
        return;
    }
    float distance = INFINITY;
    float intersectDist;
    int nearObj = 0;
    for (int num = 0; num <= numObjects; num++) {
        if (objects[num].type == PLANE) {
            intersectDist = planeIntersection(origin, direction, objects[num].position, objects[num].value.normal);
        }       
        else {
            intersectDist = sphereIntersection(origin, direction, objects[num].position, objects[num].value.radius);
        }
        if (intersectDist < distance) {
            distance = intersectDist;
            nearObj = num;
        }
    }
    if (distance == INFINITY) {  
        return;
    }

    /*TODO: Project 5 

////////reflect pseudocode from book///////////

float reflect(objects[object], x, vectorOne, int level) //not sure what level is
	{
		if (level > 7) //should recurse 7 times at the most
			return black;
		else{
			reflectionVect = v3_reflect(x, object, surfNum);
			(object, distance) = intersect(x, reflectionVect);   
			if (distance == INFINITY)
				color = backgroundcolor;
			else{
				m_color = shoot(object, x + distance * reflectionVect, reflectionVect, level + 1); //again, not sure what level is supposed to be
				color = illuminate(object, x, vectorOne, m_color, -(reflectionVect));
			}
			for(Light = 0, Lights += 1, Lights < 129) //lights in the scene...there are 128 lights if im not mistaken
				if(light i visible from x) //not sure how to write this
					color += illuminate(object, x, vectorOne, lights[i].color, light[i].direction);
			return color;
			}
		}
////////////////////////////////////////////////



    if (objects[nearObj].reflect > 0) {
        // set new ro at intersection and rd which is the reflection vector
        shoot(origin, direction, reflectionColor, recLevel - 1);
    }

    assert(objects[nearObj].reflect + objects[nearObj].refract <= 1);
    //float opacity = 1 - objects[nearObj].reflect - objects[nearObj].reflect;
    */
    float opacity = 1; //temp value
    if (opacity > 0) { 
        for (int lightNum = 0; lightNum <= numLights; lightNum++) {
            // new origin
            setArray(origin, direction);
            v3_scale(origin, distance);

            // new direction
            v3_subtract(direction, lights[lightNum].position, origin);
            v3_normalize(direction, direction);

            illuminate(origin, direction, color, nearObj, lightNum);
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
    
    //printObjects(); //remove later

    // create image
    PPM *image = (PPM*)malloc(sizeof(PPM));
    image->pixData = (unsigned char*)malloc(width*height*sizeof(Pixel)); 
    image->maxColVal = MAX_COLOR;  
    image->width = width;
    image->height = height;

    float pixHeight = camHeight/image->height;
    float pixWidth = camWidth/image->width;
    float finalColor[3] = {0,0,0};

    for(int row = 0; row < image->height; row++) {

        for(int col = 0; col < image->width; col++) {
            float origin[3] = {0,0,0};
            float direction[3] = {origin[0] - (camHeight / 2) + (pixHeight * (row+0.5)),
                                  origin[1] - (camWidth / 2) + (pixWidth *  (col+0.5)),
                                  -1};

            v3_normalize(direction, direction); 
            setArray(finalColor, backgroundColor);
            
            shoot(origin, direction, finalColor, 7); 
            
            int location = (col*image->width*3)+(row*3);
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

