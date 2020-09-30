/////////////   MAIN  ///////////////////////////////
int main (int argc, char *argv[]) {

    // number of arguments is incorrect
    if (argc != 4) {
       fail("Invaild number of arguments", IN_ERR_CODE, NULL);
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[1]);
    char *input = argv[2];
    char *output = argv[3];

    // First argument given is invalid
    if (width <= 0) {
        fail( "Invalid Width argument", IN_ERR_CODE, NULL);
    }
    else if (height <= 0) {
        fail( "Invalid Height argument", IN_ERR_CODE, NULL);
    }
    // Second Argument: file does not exist
    else if (!fileExists(input)) {
        fail("input file does not exist", IN_ERR_CODE, NULL);
    }
    // Second Argument: incorrect type
    else if(!isFileType(input, ".json")) {
        fail("input file is not of type .ppm", IN_ERR_CODE, NULL);
    }
    // Third Argument: incorrect type
    else if(!isFileType(output, ".ppm")) {
        fail("output file is not of type .ppm", IN_ERR_CODE, NULL);
    }

    return(0);
}
