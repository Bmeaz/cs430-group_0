#include <stdio.h>      
#include <stdlib.h>

#define MAX_COLOR 255 

typedef struct PixelStorage{ //struct to store individual pixels
unsigned int r;    
unsigned int g;
unsigned int b;
}PixelStorage;

typedef struct Image{   //struct to keep track of the data in the file
    int width;
    int height;
    int color;
    unsigned char *pixData;
} Image;

Image *buffer; 

void amountOfArgs(int);       
void exists(char *);
void checkFormat(int, char *);
Image readData(char *);
int writeData(int, char *);

int main(int argc, char *argv[]){
    int arguments = argc;               
    char *format = argv[1];
    int intFormat = atoi(format);
    char *inputFile = argv[2];
    char *outputFile = argv[3];

    amountOfArgs(arguments);                      
    exists(inputFile);
    readData(inputFile);
    checkFormat(intFormat, outputFile);

    return 0;  
}

void checkFormat(int intFormat , char *outputFile){   //checks what format the file is being converted to
    if((intFormat != 3) & (intFormat != 6)){
        fprintf(stderr, "Error: This is not a correct format for conversion");
        exit(1);
    }
    writeData(intFormat, outputFile);
}

void amountOfArgs(int arguments){     //checks if number of arguments is correct
    if (arguments != 4){
        fprintf(stderr, "Error: needs 4 arguments\n");
        exit(1);
    }
}

void check_format(int intFormat , char *outputFile){   //checks what format the file is being converted to
    if((intFormat != 3) & (intFormat != 6)){
        fprintf(stderr, "Error: This is not a correct format for conversion");
        exit(1);
    }
    writeData(intFormat, outputFile);
}

Image readData(char *inputFile){
    buffer = (Image*)malloc(sizeof(Image));     //allocates memory for buffer
    FILE* ifp = fopen(inputFile, "rb");        //opens input file for reading
    int current = fgetc(ifp);                   //sets variable equal to first character in file
    if(current != 'P'){
        fprintf(stderr, "Error: This is not a PPM file!\n");    //checks if file is a a valid ppm
        fclose(ifp);
        exit(1);
    }

    current = fgetc(ifp);                                       //if the file is a ppm, checks if p3 or p6
    int ppmType = current;
    if((ppmType != '3') & (ppmType != '6')){
        fprintf(stderr, "Error: Only P3/P6 files allowed!\n"); //file is not a p3 or p6
        fclose(ifp);
        exit(1);
    }

    }
    current = fgetc(ifp);

    while(current == '#'){      //skips comments
        while(current != '\n'){
            current = fgetc(ifp);
        }
        current = fgetc(ifp);
    }
    ungetc(current, ifp);

    int dimensions = fscanf(ifp, "%d %d", &buffer->width, &buffer->height);     //saves image width and height in buffer
    if(dimensions != 2){                                                        //check if dimensions are valid 
        fprintf(stderr, "Error: Dimensions not valid\n");
        fclose(ifp);
        exit(1);
    }

    int maxColor = fscanf(ifp, "%d", &buffer->color);      //checks if max color value in range
    if((maxColor < 1) | (maxColor > MAX_COLOR)){
        fprintf(stderr, "Error: The color value range is 1 through 255, please enter a valid value\n");
        fclose(ifp);
        exit(1);
    }

    buffer->pixData = (unsigned char*)malloc((buffer->width) * (buffer->height) * (sizeof(PixelStorage)));    //allocates memory for the buffer data
    if(buffer == NULL){
        fprintf(stderr, "Error: memory allocation unsuccessful\n");
        fclose(ifp);
        exit(1);
    }

    