#include "ppmrw.h"

const int VALID_FORMS[] = {3, 6};   // Vaild types of forms accepted
char PPM_IMAGE[4] = ".ppm";       // Valid type of input file

////////////////////////////   inValidInput  /////////////////////////////
void fail (char *errMsg, const int errCode) {

    fprintf(stderr, "\nError: %s\n\n", errMsg);

    // if error caused by stoopid user input
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


/////////////////////////////   readPPM ////////////////////////////////////
char *readPPM(char *filename, int form) {
     FILE *file = fopen(filename, "r");
     char curForm[MAX_STR_LEN];
     sprintf(curForm, "P%d", form); 
     char buffer[MAX_STR_LEN];
     int scanline = 0;
     
     // scan each line of the file
     while(fscanf(file, "%s", buffer) != EOF ) {

         // First Line: Checks if first line in header is the same as form
         if (scanline == 0 && strcmp(buffer, curForm) != 0) {
            return("Header in ppm does not match the form given");
         }
         scanline++;
     }
     return EMPTY_STR; 
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
    strcpy(outMsg, readPPM(input, form));
    
    //  if PPM file failed to read
    if (strcmp(outMsg, EMPTY_STR) != 0) {
        fail(outMsg, PPM_ERR_CODE);
    }
    
    printf("\nEnd of Program\n");
    return VALID_CODE;
}



