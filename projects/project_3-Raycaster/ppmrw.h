#ifndef PPMRW_H_INCLUDE
#define PPMRW_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// structure for a PPM file
typedef struct PPM {
    int form;
    int width;
    int height;
    int maxColVal;
    unsigned char *pixData;
} PPM;

// structure for a single Pixel
typedef struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel;

void fail (char *errMsg);

bool fileExists (char *filename);

bool isFileType (char *filename, char *fileType);

bool isValidForm (int form);

void writeP3(struct PPM ppm, char* outFile);

void writeP6(struct PPM ppm, char* outFile);

#endif
