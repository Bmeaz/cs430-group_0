#include "v3test.h"

// Initialize test arrays
float testArrayA [3] = {1.0,2.0,3.0};
float testArrayB [3] = {4.0,5.0,6.0};
float finalTestArray[3];

void v3_from_points_test (float *a, float *b){
    v3_from_points(finalTestArray, a, b);
    if(finalTestArray[0] != 3.0 ||
       finalTestArray[1] != 3.0 ||
       finalTestArray[2] != 3.0){
           printf("Points Test Failed: Wrong value stored in array");
           exit(1);
    }
    printf("Points Test Passed");
}

void v3_add_test (float *a, float *b){
    v3_add(finalTestArray, a, b);

    if(finalTestArray[0] != 5.0 ||
       finalTestArray[1] != 7.0 ||
       finalTestArray[2] != 9.0){
           printf("Add Test Failed: Wrong value stored in array");
           exit(1);
       }
    printf("Add Test Passed");
}

void v3_subtract_test (float *a, float *b){
    v3_subtrat(finalTestArray, a, b);

    if(finalTestArray[0] != -3.0 ||
       finalTestArray[1] != -3.0 ||
       finalTestArray[2] != -3.0){
           printf("Subtract Test Failed: Wrong value stored in array");
           exit(1);
    }
    printf("Subtract Test Passed");
}

float v3_dot_product_test (float *a, float *b){
    float finalResult = v3_dot_product(testArrayA,testArrayB);
    if(finalResult != 32){
        printf("Dot Product Test Failed: Wrong value computed");
        exit(1);
    }
    printf("Dot Product Test Passed");
}

void v3_cross_product_test (float *a, float *b){
    v3_cross_product(finalTestArray, testArrayA, testArrayB);
    if(finalTestArray[0] != -3.0 ||
       finalTestArray[1] != 6.0 ||
       finalTestArray[2] != -3.0){
           printf("Cross Product Test Failed: Wrong value stored in array");
           exit(1);
    }
    printf("Cross Product Test Passed");
}

void v3_scale_test (float s){
    v3_scale(testArrayA, 2);
    if(testArrayA[0] != 2.0 ||
       testArrayA[1] != 4.0 ||
       testArrayA[2] != 6.0){
           printf("Scale Test Failed: Wrong value stored in array");
           exit(1);
    }
    printf("Scale Test Passed");
}

void v3_angle_test (float *a, float *b){
    v3_angle(testArrayA, testArrayB);

    if(A[0] == 0 || A[1] == 0 || A[2] ==0 ||
       B[0] == 0 || B[1] == 0 || B[2] ==0 || ){
           printf("Angle Test Failed: 0 value found in array");
           exit(1);
       }
    printf("Angle Test Passed");
}

void v3_angle_quick_test (float *a, float *b){
    v3_angle_quick(testArrayA, testArrayB);

    //Checks to see if any values were not changed from 0
    if(A[0] == 0 || A[1] == 0 || A[2] ==0 ||
       B[0] == 0 || B[1] == 0 || B[2] ==0 || ){
           printf("Angle Test Failed: 0 value found in array");
           exit(1);
    }
    //Checks to see if there are duplicate values for each pair of indices
    if(A[0] == B[0] || A[1] == B[1] || A[2] == B[2]){
           printf("Angle Test Failed: 0 value found in array");
           exit(1);
    }
    printf("Angle Quick Test Passed");
}

void v3_reflect_test (float *dst, float *v, float *n){

}

float v3_length_test (float *a){
    float finalResult = v3_length(testArrayA);
    // Result should be ~ 3.74
    if(finalResult > 3.8 || finalResult < 3.7){
        printf("Length Test Failed: Value out of expected range");
        exit(1);
    }
    printf("Length Test Passed");
}

void v3_normalize_test (float *dst, float *a){

}

void resetArrays(){
    testArrayA [3] = {1.0,2.0,3.0};
    testArrayB [3] = {4.0,5.0,6.0};
    finalTestArray[3] = {0,0,0};
}

int main (int argc, char *argv[]){

    v3_add_test(testArrayA, testArrayB);
    //reset arrays back to test values
    resetArrays();
    v3_subtract_test (float *a, float *b);
    resetArrays();
    v3_dot_product_test (float *a, float *b);
    resetArrays();
    v3_cross_product_test (float *dst, float *a, float *b);
    resetArrays();
    v3_scale_test(float s);
    resetArrays();
    v3_angle_test (float *a, float *b);
    resetArrays();
    v3_angle_quick_test (float *a, float *b);
    resetArrays();
    v3_reflect_test (float *dst, float *v, float *n);
    resetArrays();
    v3_length_test (float *a);
    resetArrays();
    v3_normalize_test (float *dst, float *a);
    resetArrays();
    return 0;
}
