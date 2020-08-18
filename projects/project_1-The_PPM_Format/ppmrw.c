#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ppmrw.h"

int VALID_FORMS[] = {3, 6};    //Vaild types of forms accepted
char VALID_TYPES[4] = ".ppm"; //Valid type of input file

////////////////////////////   validArgCnt  //////////////////////////////
bool validArgCnt( int argc ) {
    bool isValid = true;
    
    // incorrect number of arguments
    if( argc != 4 ) {
       fprintf( stderr, "Error: Incorrect number of arguments given\n\n");
       printf("   Arguments should be: ./ppmrw <format> <input file> <output file>\n");
       printf("\tExample: ./ppmrw 6 input.ppm output.ppm\n\n");
       isValid = false;
    }
    return isValid;
 }

  
////////////////////////////   isValidInput  /////////////////////////////
bool isValidInput( int form, char *input, char *output ) {
    bool isValid = false;
    
    // First Argument: incorrect form given
    if( !isValidForm( form ) ) {
       fprintf( stderr, "Error: Incorrect form value given");
       printf("This program can only use: ");
       
       // print all valid form types
       for( int curFrom = 0; curFrom < sizeof(VALID_FORMS); curFrom++ ) {
           printf("P%d ", VALID_FORMS[curFrom]);
       }
       printf("\n");
     }
     
    // Second Argument: file does not exist
    else if( !fileExists(input) ) {
        fprintf( stderr, "Error: file '%s' cannot be found\n", input);
     }
     
    // Second Argument:
    else if( !isFileType(input, VALID_TYPES) ) {
        fprintf( stderr, "Error: file '%s' is not a %s type\n", input, VALID_TYPES);
     }
     
    // Third Argument: incorrect type
    else if( !isFileType(output, VALID_TYPES) ) {
        fprintf( stderr, "Error: file '%s' is not a %s type\n", output, VALID_TYPES);
     }
     
    // All arguments valids
    else {
      isValid = true;
     }
    
    return isValid;
}


////////////////////////////   isValidForm  //////////////////////////////
bool isValidForm( int form )   {
     bool isValid = false;
     
     // loop through all valid forms
     for( int curForm = 0; curForm < sizeof(VALID_FORMS); curForm++ ) {
     
         // if form found, change flag to true
         if (form == curForm)
            {
             isValid = true;
            }
     }
     return isValid;    
}


////////////////////////////   fileExists  ///////////////////////////////
bool fileExists( char *filename ) {
     bool fileExists = false;
     FILE *file = fopen(filename, "r");
     if ( file != NULL){
         fileExists = true;
         fclose(file);
     }
     return fileExists;
}


////////////////////////////   isFileType  ///////////////////////////////
bool isFileType( char *filename, char *fileType) {
     bool isValid = true;
     int lastChar = strlen(filename)-1;
     int typeLen = strlen(fileType)-1;
     for (int curChar = 0; curChar <= typeLen; curChar ++) {     
         if (filename[lastChar-typeLen+curChar] != fileType[curChar] ) {
             isValid = false;
         }
     }
     return isValid;
}

////////////////////////////   MAIN  ///////////////////////////////
int main (int argc, char *argv[]) {
    int form;  // user given form int
    char* input;  // user given input file
    char* output; // user given output file
    
    // if valid number of arguments are given
    if( validArgCnt(argc) ) { 
        form = atoi(argv[1]);
        input = argv[2];
        output = argv[3];
        
        // all arguments given are valid
        if( isValidInput( form, input, output ) ) {
         
        }
    }
    return 1;
}

