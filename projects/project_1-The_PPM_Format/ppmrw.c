#include "ppmrw.h"

const int VALID_FORMS[] = {3, 6};   // Vaild types of forms accepted

////////////////////////////   append  /////////////////////////////
void append (char* str, char character) {
    int len = strlen(str);
    str[len] = character;
    str[len+1] = '\0';
   }

////////////////////////////   atComChar  /////////////////////////////
bool atComChar (char curChar) {
     if( curChar == '#') {
         return true;
     }
     return false;
}

////////////////////////////   charInStr  /////////////////////////////
bool charInStr (char string[], char character) {
    // check each char in string to compare with given cahr
    for (int charNum = 0; charNum < strlen(string); charNum++) {
        // if chars match return true
        if (character == string[charNum]) {
            return true;
        }
    }
    return false;
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

/////////////   P3 To P3  ///////////////////////////////
void ppmP3ToP3(struct PPM ppm, FILE* inFile, FILE* outFile) {
   //Takes a given output file and creates the PPM header
   writeHeader( outFile, 3, ppm.width, ppm.height, ppm.maxColVal);
   //TODO: write file
   //Image stored as ASCII
   //Open the file given as read so we do not change the data

}

/////////////   P3 To P6  ///////////////////////////////
void ppmP3ToP6(struct PPM ppm, FILE* inFile, FILE* outFile) {
    //Takes a given output file and creates the PPM header
   writeHeader( outFile, 6, ppm.width, ppm.height, ppm.maxColVal);
   //TODO: write file
   //Image stored as binary encoding AKA Portable Pixmap rawbits format
   //Open the file given as read so we do not change the data

}


/////////////   P6 To P3  ///////////////////////////////
void ppmP6ToP3(struct PPM ppm, FILE* inFile, FILE* outFile) {
   //Takes a given output file and creates the PPM header
   writeHeader( outFile, 3, ppm.width, ppm.height, ppm.maxColVal);
   //TODO: write file
   //Image stored as ASCII
   //Open the file given as read so we do not change the data

}

/////////////   P6 To P6  ///////////////////////////////
void ppmP6ToP6(struct PPM ppm, FILE* inFile, FILE* outFile) {
   //Takes a given output file and creates the PPM header
   writeHeader( outFile, 6, ppm.width, ppm.height, ppm.maxColVal);
   //TODO: write file
   //Image stored as binary encoding AKA Portable Pixmap rawbits format
   //Open the file given as read so we do not change the data

}


/////////////////////////////   readPPM ////////////////////////////////////
struct PPM readHeader(FILE* file) {
     // read the PPM, output output message to outMsg
     char curStr[MAX_STR_LEN];
     char curChar = ' ';
     char first = fgetc(file), second = fgetc(file), third = fgetc(file);
     bool isComment = false, endHeader = false;
     struct PPM *ppm = malloc(sizeof(PPM));
     ppm->form = (second-'0');
     ppm->height = INVALID_INT;
     ppm->width = INVALID_INT;

     // first two char are not P and form or third char is not an end character
     if (first != 'P' || !isValidForm((second)-'0') || !charInStr(SPACE_CHAR, third)) {

         fail("form type in .ppm file is not a valid type", PPM_HDR_ERR, file);
     }
     ungetc(third, file);
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
	   isComment = atComChar(curChar);

	   // current char is not endspace - append char to sting if an integer
           if (!charInStr(SPACE_CHAR, curChar)) {
               // current char is not an integer or a endspace - fail
               if (!isdigit(curChar)) {
                    free(ppm);
                    fail("invalid character found in header", PPM_HDR_ERR, file);
               }
               append(curStr, curChar); //add char to curStr
           }
           // end of current string
           else if (curStr[0] != '\0') {
               if (ppm->width == INVALID_INT) {
                   ppm->width = atoi(curStr);
               }
	       // current string is the height
	       else if (ppm->height == INVALID_INT) {
	           ppm->height = atoi(curStr);
	           clearStr(curStr);
	       }
               // current string is the maximum color value
               else {
                   ppm->maxColVal = atoi(curStr);
                   endHeader = true;
               }
               clearStr(curStr);
           }
        }
     } // end while loop for header

     // Not enough information in header
     if (!endHeader) {
        free(ppm);
        fail("incorrect header form, missing a value or endspace", PPM_HDR_ERR, file);
     }
     // invalid information in header
     else if (ppm->height < 0 || ppm->width < 0 || ppm->maxColVal < 0 || ppm->maxColVal > MAX_COLOR) {
        free(ppm);
        fail("a value in the header file in not a valid positive integer", PPM_HDR_ERR, file);
     }

     return *ppm;
}

/////////////   writeHeader  ///////////////////////////////
void writeHeader(FILE* file, int form, int width, int height, int maxCol) {
    char hdr[MAX_STR_LEN];
    //places Magic number and dimensions in header
    sprintf(hdr, "P%d\n%d %d\n%d\n",  form, width, height, maxCol);
    fputs(hdr, file);
}

/////////////   MAIN  ///////////////////////////////
int main (int argc, char *argv[]) {

    // number of arguments is incorrect
    if (argc != 4) {
       fail("Invaild number of arguments", IN_ERR_CODE, NULL);
    }
    int outForm = atoi(argv[1]);
    char *input = argv[2];
    char *output = argv[3];

    // argument given is invalid
    if (!isValidForm(outForm)) {
        fail( "Incorrect form value, This program can only use: P3 and P6", IN_ERR_CODE, NULL);
    }
    // Second Argument: file does not exist
    else if (!fileExists(input)) {
        fail("input file does not exist", IN_ERR_CODE, NULL);
    }
    // Second Argument: incorrect type
    else if(!isFileType(input, ".ppm")) {
        fail("input file is not of type .ppm", IN_ERR_CODE, NULL);
    }
    // Third Argument: incorrect type
    else if(!isFileType(output, ".ppm")) {
        fail("output file is not of type .ppm", IN_ERR_CODE, NULL);
    }

    FILE *inFile = fopen(input, "r");
    struct PPM ppm = readHeader(inFile);
    //TODO: validate the rest of the input file

    FILE *outFile = fopen(output, "w");

    // read file if ppm 3 form
    if (ppm.form == 3 && outForm == 3) {
        ppmP3ToP3(ppm, inFile, outFile);
    }
    // read file if ppm 6 form
    else if (ppm.form == 3 && outForm == 6) {
        ppmP3ToP6(ppm, inFile, outFile);
    }
    // write file if ppm 3 form
    if (ppm.form == 6 && outForm == 3) {
        ppmP6ToP3(ppm, inFile, outFile);

    }
    // write file if ppm 6 form
    else if (ppm.form == 6 && outForm == 6) {
        ppmP6ToP6(ppm, inFile, outFile);

    }

    fclose(inFile);
    fclose(outFile);
    printf("\nEnd of Program\n");
    return VALID_CODE;
}
