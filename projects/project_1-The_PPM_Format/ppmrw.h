#ifndef PPMRW_H_INCLUDE
#define PPMRW_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

char EMPTY_STR[3] = "   ";    // Empty sting
char WRITE_SPACE = ' ';
char SPACE_CHAR[6] = {' ', '\n', '\t', '\r', '#', '\0'};  // end Characters

const int VALID_CODE = 0;     // Success code
const int ERR_CODE = 1;       // Default error code
const int IN_ERR_CODE = -1;   // Input error code
const int PPM_HDR_ERR = -2;   // PPM head error code
const int PPM_DATA_ERR = -3;  // PPM data error code
const int MAX_STR_LEN = 100;  // Maximum string length
const int MAX_COLOR = 255;    // Maximum color
const int INVALID_INT = -999; // Invalid integer

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

/////////////////////////////////   append  ///////////////////////////////
// appends given char to given string and returns string
// parameters: 
//      char *str: string that needs appending
//      char character: character that will be appended
////////////////////////////////////////////////////////////////////////// 
void append (char* str, char character);


////////////////////////////   atComChar  /////////////////////////////
// checks if char is at the comment char
// parameters: 
//      char curChar: character to check if a comment
////////////////////////////////////////////////////////////////////////// 
bool atComChar (char curChar);


////////////////////////////   charInStr  ///////////////////////////////
// checks if given char is in given char array
// parameters: 
//      char string[]: array of char
//      char character: given character
// return:
//      bool output: if chararter is in string[]
////////////////////////////////////////////////////////////////////////// 
bool charInStr (char string[], char character);


////////////////////////////   clearStr  /////////////////////////////  
// clears a char array
// parameters: 
//      char str[]: char array to be cleared
////////////////////////////////////////////////////////////////////////// 
void clearStr (char str[]);


/////////////////////////////////   fail  ///////////////////////////////
// prints error message and exits program
// parameters: 
//      char *errMsg: string containing error message
//      const int errCode: value of type of error
//      FILE* file: file to close
////////////////////////////////////////////////////////////////////////// 
void fail (char *errMsg, const int errCode, FILE* file);


////////////////////////////   fileExists  ///////////////////////////////
// checks if given file exists and is in the same directory
// parameters: 
//      char *filename: name of the file
// return:
//      bool fileExists: true if file is found, false otherwise
////////////////////////////////////////////////////////////////////////// 
bool fileExists (char *filename);


////////////////////////////   isFileType  ///////////////////////////////
// checks if given file is the same type as type given
// parameters: 
//      char *filename: name of the file
//      char *fileType: suffix for filetype
// return:
//      bool isValid: true if file ends in the suffix given for the filetype, false otherwise
////////////////////////////////////////////////////////////////////////// 
bool isFileType (char *filename, char *fileType);


////////////////////////////   isValidForm  //////////////////////////////
// checks if given form is a vaild for that the program can use
// parameters: 
//      int form: PPM form form 
//                valid form are found at top of file (VALID_TYPES)
// return:
//      bool isValid: true if valid form, false otherwise
////////////////////////////////////////////////////////////////////////// 
bool isValidForm (int form);


////////////////////////////   readFile  //////////////////////////////
// reads input file and returns structure ppm containing the information
// parameters: 
//      char* filename: name of input file
// return:
//      struct PPM ppm: structure containing image information
////////////////////////////////////////////////////////////////////////// 
struct PPM readFile(char* filename);


////////////////////////////   writeP3  //////////////////////////////
// writes ppm infomation to output file in ppm P3 format
// parameters: 
//      struct PPM ppm: structure containing image information
//      char* outFile: name of output file
////////////////////////////////////////////////////////////////////////// 
void writeP3(struct PPM ppm, char* outFile);


////////////////////////////   writeP6  //////////////////////////////
// writes ppm infomation to output file in ppm P6 format
// parameters: 
//      struct PPM ppm: structure containing image information
//      char* outFile: name of output file
////////////////////////////////////////////////////////////////////////// 
void writeP6(struct PPM ppm, char* outFile);


////////////////////////////   main  ///////////////////////////////
// runs program
// parameters: 
//      int argc: number of arguments
//      char *argv[]: arguments
// return:
//      int: returns integer
////////////////////////////////////////////////////////////////////////// 
int main (int argc, char *argv[]);


#endif
