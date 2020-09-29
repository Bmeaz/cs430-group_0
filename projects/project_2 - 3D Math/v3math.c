#include "v3math.h"

void v3_from_points(float *dst, float *a, float *b) {
    dst[0] = b[0] - a[0];
    dst[1] = b[1] - a[1];
    dst[2] = b[2] - a[2];
}

void v3_add(float *dst, float *a, float *b) {
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}

void v3_subtract(float *dst, float *a, float *b) {
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
}

float v3_dot_product(float *a, float *b) {
    return (a[0]*b[0]) + (a[1]*b[1]) + (a[2]*b[2]);
}

void v3_cross_product(float *dst, float *a, float *b) {
    dst[0] = (a[1] * b[2]) - (a[2] * b[1]);
    dst[1] = (a[2] * b[0]) - (a[0] * b[2]);
    dst[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

void v3_scale(float *dst, float s) {
    dst[0] = s * dst[0];
    dst[1] = s * dst[1];
    dst[2] = s * dst[2];
}


float v3_angle(float *a, float *b){
    return acosf(v3_angle_quick(a, b));
}

// angle between a and b; no cos-1
float v3_angle_quick(float *a, float *b){
    float a_unit[3] = {0.0, 0.0, 0.0};
    float b_unit[3] = {0.0, 0.0, 0.0};
    v3_normalize(a_unit, a);
    v3_normalize(b_unit, b);
    return v3_dot_product(a_unit, b_unit);
}


// From notes in class:
// Vr = v + 2b
// b = -(n*v) * n
// Vr = v + -2 (n*v) * n
void v3_reflect(float *dst, float *v, float *n) {

    for(int refIndex = 0; refIndex < 3; refIndex++){
        // Compute b at the start of the loop
        float b = -(n[refIndex])*(v[refIndex]) * (n[refIndex]);
        //Compute Vr by using previously calculated b
        dst[refIndex] = v[refIndex] + 2*b;
    }


}

float v3_length(float *a){
    return sqrtf((a[0] * a[0]) + (a[1] * a[1]) + (a[2] * a[2]));
}

void v3_normalize(float *dst, float *a) {
    float len = v3_length(a);
    assert(len != 0);
    dst[0] = a[0]/len;
    dst[1] = a[1]/len;
    dst[2] = a[2]/len;
}

bool v3_equals(float *a, float *b, float tolerance) {

    if(fabs(a[0] - b[0]) > tolerance ||
       fabs(a[1] - b[1]) > tolerance  ||
       fabs(a[2] - b[2]) > tolerance) {
        return false;
    }
    return true;
}
