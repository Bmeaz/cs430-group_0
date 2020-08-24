#include "ppmrw.h"

const int VALID_FORMS[] = {3, 6};   // Vaild types of forms accepted
char PPM_IMAGE[4] = ".ppm";       // Valid type of input file

////////////////////////////   append  /////////////////////////////
void append (char* str, char character) {
    int len = strlen(str);
    str[len] = character;
    str[len+1] = '\0';
   }

////////////////////////////   charInStr  /////////////////////////////
bool charInStr (char string[], char character) {
    bool output = false;
    // check each char in string to compare with given cahr
    for (int charNum = 0; charNum < strlen(string); charNum++) {
        // if chars match return true
        if (character == string[charNum]) {
            output = true;
        }
    }
    return output;
}       

////////////////////////////   clearStr  /////////////////////////////  
void clearStr(char str[]) {
     sprintf(str, "%c", '\0'); //clear current string
}

////////////////////////////   fail  /////////////////////////////
void fail (char *errMsg, const int errCode, FILE* file) {
    // close file
    if (file != NULL) {
        fclose(file);
    }
    fprintf(stderr, "\nError: %s\n\n", errMsg);
    // error caused by stoopid user input
    if (errCode == IN_ERR_CODE) {
        printf("\tArguments should be: ./ppmrw <format> <input> <output>\n");
        printf("\t\tExample: ./ppmrw 6 input.ppm output.ppm\n\n");
    }
    // error code caused by invaild header
    else if (errCode == PPM_HDR_ERR) {
        printf("\tHeader should contain form, width, height and max color value\n");
        printf("\t\tExample: P6 500 500 255\n\n");
    }
    exit(ERR_CODE);
}

////////////////////////////   fileExists  ///////////////////////////////
bool fileExists (char *filename) {
     bool fileExists = false;
     FILE *file = fopen(filename, "r");
     // opens file and checks if it exists
     if (file != NULL) {
         fileExists = true;
         fclose(file);
     }
     return fileExists;
}

////////////////////////////   isFileType  ///////////////////////////////
bool isFileType (char *filename, char *fileType) {
    char *curFileType = &filename[strlen(filename) - strlen(fileType)];
    return strcmp(curFileType, fileType) == 0;
}

////////////////////////////   isValidForm  //////////////////////////////
bool isValidForm (int form)   {
     bool isValid = false;
     int numForms = sizeof(VALID_FORMS)/(VALID_FORMS[0]);
     // loop through all valid forms
     for( int curForm = 0; curForm < numForms; curForm++ ) {
         // if form found, change flag to true
         if (form == VALID_FORMS[curForm]) {
             isValid = true;
         }
     }
     return isValid;
}
   
////////////////////////////   isValidInput  /////////////////////////////
char *isValidInput (int form, char *input, char *output) {
    char *outStr;
    char msg[MAX_STR_LEN];
    // First Argument: incorrect form given
    if (!isValidForm(form)) {
       sprintf(msg, "Incorrect form value, This program can only use: P3 and P6");
    }
    // Second Argument: file does not exist
    else if (!fileExists(input)) {
        sprintf(msg, "file '%s' cannot be found", input);
    }
    // Second Argument: incorrect type
    else if(!isFileType(input, PPM_IMAGE)) {
        sprintf(msg, "file '%s' is not a '%s' type", input, PPM_IMAGE);
    }
    // Third Argument: incorrect type
    else if(!isFileType(output, PPM_IMAGE)) {
        sprintf(msg, "file '%s' is not a '%s' type", output, PPM_IMAGE);
    } 
    // All arguments are valid
    else {
       return EMPTY_STR;
    }
    outStr = msg;
    return outStr;
}


////////////////////////////   validInt  //////////////////////////////
int validInt (char string[]) {
    bool isValid = true;
    int intVal = PPM_HDR_ERR;
    int curChar = 0;
    //loop through the string
    while (string[curChar] != '\0') {
        // if invalid number found, flip flag to false
        if (!isdigit(string[curChar])) {
           isValid = false;
        }
        curChar ++;
    }
    // if valid convert string to int
    if (isValid) {
       intVal = atoi(string);
    }
    return intVal;
}


////////////////////////////   MAIN  ///////////////////////////////
int main (int argc, char *argv[]) {
    char outMsg[MAX_STR_LEN];  // output message

    // number of arguments is incorrect
    if (argc != 4) {
       fail("Invaild number of arguments", IN_ERR_CODE, NULL);
    }
    int outForm = atoi(argv[1]);
    char *input = argv[2];
    char *output = argv[3];
    strcpy(outMsg, isValidInput(outForm, input, output));

    // argument given is invalid
    if (strcmp(outMsg, EMPTY_STR) != 0) {
       fail(outMsg, IN_ERR_CODE, NULL);
    }
    // read the PPM, output output message to outMsg
     FILE *file = fopen(input, "r");
     char curStr[MAX_STR_LEN];
     char curChar = ' ';
     char first = fgetc(file), second = fgetc(file), third = fgetc(file);
     bool isComment = false, endHeader = false;
     int headerVal = 0, width = 0, height = 0, maxColVal = 0;
     
     // first two char are not P and form or third char is not an end character    
     if (first != 'P' || !isValidForm(second) || !charInStr(SPACE_CHAR, third)) {
         fail("form type in .ppm file is not a valid type", PPM_HDR_ERR, file);
     }
     // third char is comment, set flag
     if( third == '#') {
         isComment = true;
     }
     clearStr(curStr);

     // scan file for header info
     while (!endHeader && curChar != EOF) {
        curChar = fgetc(file);
        // current char is the end of a comment
        if (curChar == '\n' && isComment) {
           isComment = false; 
           clearStr(curStr);
        }
        // current character is not a space or we are within comment
        else if (!isComment) {
	   
	   // current char in file is comment - set flag
           if (curChar == '#') {
               isComment = true;
           }
	   // current char is not endspace - append char to sting if an integer
           if (!charInStr(SPACE_CHAR, curChar)) {
               // current char is not an integer or a endspace - fail
               if (validInt(curStr) == PPM_HDR_ERR) {
                    fail("invalid character found in header", PPM_HDR_ERR, file);
               }
               append(curStr, curChar); //add char to curStr
           }
           // end of current string
           else if (validInt(curStr) >= 0) {
               // current string is the width
               if (headerVal == 0) { 
                   width = validInt(curStr);
                   headerVal ++;
               }
	       // current string is the height
	       else if (headerVal == 1) {
	           height = validInt(curStr);
	           headerVal ++;
	       }
               // current string is the maximum color value
               else if (headerVal == 2) {
                   maxColVal = validInt(curStr);
                   endHeader = true;
               }
               clearStr(curStr);
           }
        }
     } // end while loop for header
     
     // Not enough information in header
     if (!endHeader) {
        fail("incorrect header form, missing a value or endspace", PPM_HDR_ERR, file);
     } 
     // invalid information in header
     else if (height <= 0 || width <= 0 || maxColVal < 0 ) {
        fail("a value in the header file in not a valid positive integer", PPM_HDR_ERR, file);
     }
     
     // read file if ppm 3 form
     if ((int)second == 3) {
         //TODO: read file in ppm 3 form
     }
     // read file if ppm 6 form
     else if ((int)second == 6) {
         //TODO: write file in ppm 6 form
     }
     
     // write file if ppm 3 form
     if (outForm == 3) {
         //TODO: write file in ppm 3 form
     } 
     // write file if ppm 6 form
     else if (outForm == 6) {
         //TODO: write file in ppm 6 form
     }

     fclose(file);
    
    printf("\nEnd of Program\n");
    
    return VALID_CODE;
}

