#ifndef PPMRW_H_INCLUDE
#define PPMRW_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

char EMPTY_STR[3] = "   ";       // Empty sting

const int VALID_CODE = 0;     // Success code
const int ERR_CODE = 1;       // Default error code
const int IN_ERR_CODE = 2;    // Input error code
const int PPM_ERR_CODE = 3;
const int MAX_STR_LEN = 255;  // Maximum string length
const int MAX_COLOR = 255;    // Maximum color

/////////////////////////////////   fail  ///////////////////////////////
// prints error message and exits program
// parameters: 
//      char *errMsg: string containing error message
//      const int errCode: value of type of error
////////////////////////////////////////////////////////////////////////// 
void fail (char *errMsg, const int errCode);

////////////////////////////   isValidInput  /////////////////////////////
// checks every user argument to ensure they are vaild
// parameters: 
//      int form: PPM form form 
//                valid form are found at top of file (VALID_TYPES)
//      char *input: input file given by the user
//                needs to be in form of .ppm and exist in same directory
//      char *output: output file given by the user
//                needs to be in form of .ppm
// return:
//      char* outStr: sting containing the errMsg, empty string if no error
////////////////////////////////////////////////////////////////////////// 
char *isValidInput (int pattern, char *input, char *output);


////////////////////////////   isValidForm  //////////////////////////////
// checks if given form is a vaild for that the program can use
// parameters: 
//      int form: PPM form form 
//                valid form are found at top of file (VALID_TYPES)
// return:
//      bool isValid: true if valid form, false otherwise
////////////////////////////////////////////////////////////////////////// 
bool isValidForm (int form);


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


////////////////////////////   readPPM  ///////////////////////////////
// reads the PPM and checks if it is valid
// parameters: 
//      char *filename: name of the ppm file
//      int form: the number of the type of form
// return:
//      char* string of the output message
////////////////////////////////////////////////////////////////////////// 
char *readPPM(char *filename, int form);

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
