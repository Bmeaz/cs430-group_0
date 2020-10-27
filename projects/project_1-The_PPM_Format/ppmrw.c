#include "ppmrw.h"

const int FORMS[] = {3, 6};   // Vaild types of forms accepted

////////////////////////////   append  /////////////////////////////
void append (char* str, char character) {
    int len = strlen(str);
    str[len] = character;
    str[len+1] = '\0';
}

////////////////////////////   atComChar  /////////////////////////////
bool atComChar (char curChar) {
     return curChar == '#';
}

////////////////////////////   charInStr  /////////////////////////////
bool charInStr (char string[], char character) {
    // check each char in string to compare with given cahr
    for (int charNum = 0; charNum < strlen(string); charNum++) {
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
     // loop through all valid forms and compare with input
     for( int curForm = 0; curForm < sizeof(FORMS)/(FORMS[0]); curForm++ ) {
         if (form == FORMS[curForm]) {
             return true;
         }
     }
     return false;
}

/////////////////////////////   readPPM ////////////////////////////////////
struct PPM readFile(char* filename) {
     // read the PPM, output output message to outMsg
     FILE *file = fopen(filename, "r");
     char curStr[MAX_STR_LEN], curChar = ' ';
     bool isComment = false, endHeader = false;
     char firstChar = fgetc(file);

     struct PPM *ppm = malloc(sizeof(PPM));
     ppm->form = fgetc(file)-'0';

     // first two char are not P and form or third char is not an end character
     if (firstChar != 'P' || !isValidForm(ppm->form)) {
         fail("form type in .ppm file is not a valid type", PPM_HDR_ERR, file);
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
        else if (!isComment) {
	   isComment = atComChar(curChar);

	   // current char is not endspace - append char to sting if an integer
           if (!charInStr(SPACE_CHAR, curChar)) {
               if (!isdigit(curChar)) {
                    free(ppm);
                    fail("invalid character found in header", PPM_HDR_ERR, file);
               }
               append(curStr, curChar);
           }
           // end of current string
           else if (curStr[0] != '\0') {
               if (ppm->width == INVALID_INT) {
                   ppm->width = atoi(curStr);
               }
	       else if (ppm->height == INVALID_INT) {
	           ppm->height = atoi(curStr);
	           clearStr(curStr);
	       }
               else {
                   ppm->maxColVal = atoi(curStr);
                   endHeader = true;
               }
               clearStr(curStr);
            }
        }
    } // end while loop for header

    // Not enough information in header
    if (!endHeader || ppm->height < 0 || ppm->width < 0 ||
        ppm->maxColVal < 0 || ppm->maxColVal > MAX_COLOR) {
        free(ppm);
        fail("Invalid header, ensure that the header does not have invalid characters", PPM_HDR_ERR, file);
    }

    ppm->pixData = (unsigned char*)malloc((ppm->width) * (ppm->height) * (sizeof(Pixel)));    //allocates memory for the buffer data

    // read image data for P3 image file
    if (ppm->form == 3) {
        for(int col = 0; col < ppm->height; col++) {
            for(int row = 0; row < ppm->width; row++) {
                int location = col * ppm->width * 3 + row * 3;
                Pixel *pixel = (Pixel*)malloc(sizeof(Pixel));

                if (fscanf(file, "%hhd %hhd %hhd ", &pixel->red, &pixel->green, &pixel->blue) == EOF) {
                    fail("Input file has less data than expected", PPM_DATA_ERR, file);
                }
                ppm->pixData[location] = pixel->red;
                ppm->pixData[location + 1] = pixel->green;
                ppm->pixData[location + 2] = pixel->blue;
            }
        }
    }
    // read image data for P6 image file
    else if (ppm->form == 6) {
        int imageSize = ppm->width * ppm->height;
        size_t size = fread(ppm->pixData, sizeof(struct Pixel)/4, imageSize, file);   //saves p6 data

        if(size < imageSize) {
            fail("Input file has less data than expected", PPM_DATA_ERR, file);
        }

        for(int col = 0; col < ppm->height; col++){
            for(int row = 0; row < ppm->width; row++){
                int location = fread(ppm->pixData,sizeof(struct Pixel), imageSize, file);

                Pixel *pixel = (Pixel*)malloc(sizeof(struct Pixel));
                ppm->pixData[location] = pixel->red;
                ppm->pixData[location + 1] = pixel->green;
                ppm->pixData[location + 2] = pixel->blue;
            }
        }

    }
    // fail if more data found after image is read
    while (curChar != EOF) {
        if (!charInStr(SPACE_CHAR, curChar)) {
            free(ppm);
            fail("Input file has more data than expected", PPM_DATA_ERR, file);
        }
        curChar = fgetc(file);
    }
    return *ppm;
}

///////////// write P3  ///////////////////////////////
void writeP3(struct PPM ppm, char* outFile) {
    FILE *file = fopen(outFile, "w");
    fprintf(file, "P3\n%d %d\n%d\n", ppm.width, ppm.height, ppm.maxColVal);
    for(int col = 0; col < ppm.height; col++) {
       for(int row = 0; row < ppm.width; row++) {
           int location = col * ppm.width * 3 + row * 3;
           fprintf(file, "%d %d %d\n", ppm.pixData[location],
                                       ppm.pixData[location + 1],
                                       ppm.pixData[location + 2]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

/////////////  write P6  ///////////////////////////////
void writeP6(struct PPM ppm, char* outFile) {
    FILE *file = fopen(outFile, "wb");
    fprintf(file, "P6\n%d %d\n%d\n", ppm.width, ppm.height, ppm.maxColVal);

    fwrite(ppm.pixData, sizeof(struct Pixel), ppm.width * ppm.height, file);

    fclose(file);
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

    // First argument given is invalid
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

    struct PPM ppm = readFile(input);

    if (outForm == 3) {
        writeP3(ppm, output);
    }
    else if (outForm == 6) {
        writeP6(ppm, output);
    }

    printf("\n\tConversion Successful\n\n");
    return VALID_CODE;
}

