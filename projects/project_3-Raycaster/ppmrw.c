#include "ppmrw.h"

const int FORMS[] = {3, 6};   // Vaild types of forms accepted

////////////////////////////   fail  /////////////////////////////
void fail (char *errMsg) {
    fprintf(stderr, "\nError: %s\n\n", errMsg);
    exit(1);
}

////////////////////////////   fileExists  ///////////////////////////////
bool fileExists (char *filename) {
     bool fileExists = false;
     FILE *file = fopen(filename, "r");
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
     for( int curForm = 0; curForm < sizeof(FORMS)/(FORMS[0]); curForm++ ) {
         if (form == FORMS[curForm]) {
             return true;
         }
     }
     return false;
}

///////////// write P3  ///////////////////////////////
void writeP3(PPM *ppm, char *outFile) {
    FILE *file = fopen(outFile, "w");
    fprintf(file, "P3\n%d %d\n%d\n", ppm->width, ppm->height, ppm->maxColVal);
    for(int col = 0; col < ppm->height; col++) {
       for(int row = 0; row < ppm->width; row++) {
           int location = col * ppm->width * 3 + row * 3;
           fprintf(file, "%d %d %d\n", ppm->pixData[location],
                                       ppm->pixData[location + 1],
                                       ppm->pixData[location + 2]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

/////////////  write P6  ///////////////////////////////
void writeP6(PPM *ppm, char *outFile) {
    FILE *file = fopen(outFile, "wb");
    fprintf(file, "P6\n%d %d\n%d\n", ppm->width, ppm->height, ppm->maxColVal);

    fwrite(ppm->pixData, sizeof(Pixel), ppm->width * ppm->height, file);

    fclose(file);
}
