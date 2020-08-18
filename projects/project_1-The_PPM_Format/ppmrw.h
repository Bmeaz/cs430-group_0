#ifndef PPMRW_H_INCLUDE
#define PPMRW_H_INCLUDE


////////////////////////////   validArgCnt  //////////////////////////////
// checks if proper amount of arguments were given
// user should given for arguments
// parameters: 
//      int argc: parameter for number of arguments
// return:
//      bool isValid: true for correct num of arguments, false otherwise
//////////////////////////////////////////////////////////////////////////
bool validArgCnt( int argc );


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
//      bool isValid: true if all values are correct, false otherwise
////////////////////////////////////////////////////////////////////////// 
bool isValidInput( int pattern, char *input, char *output );


////////////////////////////   isValidForm  //////////////////////////////
// checks if given form is a vaild for that the program can use
// parameters: 
//      int form: PPM form form 
//                valid form are found at top of file (VALID_TYPES)
// return:
//      bool isValid: true if valid form, false otherwise
////////////////////////////////////////////////////////////////////////// 
bool isValidForm( int form );


////////////////////////////   fileExists  ///////////////////////////////
// checks if given file exists and is in the same directory
// parameters: 
//      char *filename: name of the file
// return:
//      bool fileExists: true if file is found, false otherwise
////////////////////////////////////////////////////////////////////////// 
bool fileExists(char *filename);


////////////////////////////   isFileType  ///////////////////////////////
// checks if given file is the same type as type given
// parameters: 
//      char *filename: name of the file
//      char *fileType: suffix for filetype
// return:
//      bool isValid: true if file ends in the suffix given for the filetype, false otherwise
////////////////////////////////////////////////////////////////////////// 
bool isFileType( char *filename, char *fileType);


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
