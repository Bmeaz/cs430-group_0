#include "ppmrw.h"

const int VALID_FORMS[] = {3, 6};   // Vaild types of forms accepted

char VALID_TYPES[4] = ".ppm"; // Valid type of input file

////////////////////////////   inValidInput //////////////////////////////
void fail (char *errMsg, const int errCode) {
    
    fprintf(stderr, "Error: %s\n\n", errMsg);
    
    // if error caused by user input
    if (errCode == IN_ERR_CODE) {
        printf("\tArguments should be: ./ppmrw <format> <input file> <output file>\n");
        printf("\t\tExample: ./ppmrw 6 input.ppm output.ppm\n\n");
    }
    exit(ERR_CODE);
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
     
    // Second Argument:
    else if(!isFileType(input, VALID_TYPES)) {
        sprintf(msg, "file '%s' is not a '%s' type", input, VALID_TYPES);
     }
     
    // Third Argument: incorrect type
    else if(!isFileType(output, VALID_TYPES)) {
        sprintf(msg, "file '%s' is not a '%s' type", output, VALID_TYPES);
     }
    
    outStr = msg;
    return outStr;
}


////////////////////////////   isValidForm  //////////////////////////////
bool isValidForm (int form)   {
     bool isValid = false;
     int numForms = sizeof(VALID_FORMS)/(VALID_FORMS[0]);
     
     // loop through all valid forms
     for( int curForm = 0; curForm < numForms; curForm++ ) {
         // if form found, change flag to true
         if (form == VALID_FORMS[curForm])
            {
             isValid = true;
            }
     }
     return isValid;    
}

////////////////////////////   fileExists  ///////////////////////////////
bool fileExists (char *filename) {
     bool fileExists = false;
     FILE *file = fopen(filename, "r");
     
     // opens file and checks if it exists
     if (file != NULL){
         fileExists = true;
         fclose(file);
     }
     return fileExists;
}


////////////////////////////   isFileType  ///////////////////////////////
bool isFileType (char *filename, char *fileType) {
     bool isValid = true;
     int lastChar = strlen(filename)-1;
     int typeLen = strlen(fileType)-1;
     
     // compare each char of the suffix of the filename to the correct filetype
     for (int curChar = 0; curChar <= typeLen; curChar ++) {     
         if (filename[lastChar-typeLen+curChar] != fileType[curChar]) {
             isValid = false;
         }
     }
     return isValid;
}

////////////////////////////   MAIN  ///////////////////////////////
int main (int argc, char *argv[]) {
    int form;       // user given form int
    char *input;    // user given input file
    char *output;   // user given output file
    char validInput[MAX_STR_LEN]; 

    // number of arguments is incorrect
    if (argc != 4) { 
       fail("Invaild number of arguments", IN_ERR_CODE);
    }
    form = atoi(argv[1]);
    input = argv[2];
    output = argv[3];
    strcpy(validInput, isValidInput(form, input, output));

    // argument given is invalid
    if (strcmp(validInput, EMPTY_STR) != 0) {
       fail(validInput, IN_ERR_CODE);
    }
    
    printf("User has valid input");
    
    return VALID_CODE;
}

