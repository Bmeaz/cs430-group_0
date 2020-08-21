#include "ppmrw.h"

const int VALID_FORMS[] = {3, 6};   // Vaild types of forms accepted
char PPM_IMAGE[4] = ".ppm";       // Valid type of input file


////////////////////////////   append  /////////////////////////////
void append (char* str, char character) {
    int len = strlen(str);
    str[len]= character;
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
           
////////////////////////////   fail  /////////////////////////////
void fail (char *errMsg, const int errCode) {

    fprintf(stderr, "\nError: %s\n\n", errMsg);

    // if error caused by stoopid user input
    if (errCode == IN_ERR_CODE) {
        printf("\tArguments should be: ./ppmrw <format> <input file> <output file>\n");
        printf("\t\tExample: ./ppmrw 6 input.ppm output.ppm\n\n");
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

/////////////////////////////   readPPM ////////////////////////////////////
void readPPM(char *filename, int form) {
     FILE *file = fopen(filename, "r");
     char curStr[MAX_STR_LEN];
     char curChar = ' ';
     bool isComment = false, endHeader = false;
     int headerVal = 0, width = 0, height = 0, maxColVal = 0;
     
     // check if first to characters are formw
     if (fgetc(file) != 'P' 
                || fgetc(file) != form +'0' 
                || !charInStr(SPACE_CHAR, fgetc(file))) {
        fail("Form type in .ppm file does not match user input", PPM_HDR_ERR);
     }
     sprintf(curStr, "%c", '\0'); //clear current string

     // scan file for header info
     while (!endHeader && curChar != EOF) {
        curChar = fgetc(file);
        
        // current character is a comment
        if (curChar == '#') {
            isComment = true;   
        }
        
        // current char is the end of a comment
        else if (curChar == '\n' && isComment) {
           isComment = false;  
           sprintf(curStr, "%c", '\0'); //clear current string
        }
        
        // current character is not a space or we are within comment
        else if (curChar == ' ' && !isComment) {
           
           // Current string is the width
           if (headerVal == 0) {
               width = validInt(curStr);
           }
           
           // current string is the height
           else if (headerVal == 1) {
               height = validInt(curStr);
           }
           
           // current str is the maximum color value
           else if (headerVal == 2) {
               maxColVal = validInt(curStr);
               endHeader = true;

           }
           headerVal ++;
           sprintf(curStr, "%c", '\0'); //clear current string
        }
        
        // char should not be in header unless in comment, fail if found
        else if (curChar >= '!' && curChar >= '~' && !isComment) {
            fail("invalid char found in ppm header", PPM_HDR_ERR);
        }
        
        // add current character to the string
        else {
           append(curStr, curChar);  
        }
        
     }

     // Not enough information in header
     if (!endHeader ) {
        fail("incorrect header form, missing a value or endspace", PPM_HDR_ERR);
     }
     
     // invalid information in header
     else if (height <= 0 || width <= 0 || maxColVal < 0 ) {
        fail("a value in the header file in not a valid positive integer", PPM_HDR_ERR);
     }
     
     // temp output for development
     printf("\n\tHeader Info [form: P%d, w: %d, h: %d, v: %d]\n", form, width, height, maxColVal);
     fclose(file);
}

////////////////////////////   validInt  //////////////////////////////
int validInt (char string[]) {
    bool isValid = true;
    int intVal = PPM_HDR_ERR;
    for (int len = 0; len < strlen(string); len++) {
        if (string[len] <= '0' && string[len] >= '9') {
           isValid = false;
        }
    }
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
       fail("Invaild number of arguments", IN_ERR_CODE);
    }
    
    int form = atoi(argv[1]);
    char *input = argv[2];
    char *output = argv[3];
    strcpy(outMsg, isValidInput(form, input, output));

    // argument given is invalid
    if (strcmp(outMsg, EMPTY_STR) != 0) {
       fail(outMsg, IN_ERR_CODE);
    }

    // read the PPM, output output message to outMsg
    readPPM(input, form);
    
    
    printf("\nEnd of Program\n");
    return VALID_CODE;
}



