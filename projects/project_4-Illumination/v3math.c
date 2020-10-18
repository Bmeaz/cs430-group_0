#include "v3math.h"

float sqr(float value) {
    return value * value;
}

void setArray(float *dst, float *a) {
    dst[0] = a[0];
    dst[1] = a[1];
    dst[2] = a[2];

}

void get_light_vector(float *dst, float *o, float *n) {
    float numerator[3] = {0,0,0};
    float denom[3] = {0,0,0};
    v3_subtract(numerator, o, n);
    setArray(denom, numerator);
    v3_normalize(denom, denom);
    v3_divide(dst, numerator, denom);
}
    
void v3_divide(float *dst, float *a, float *b) {
    for (int x = 0; x < 3; x ++) {
        assert(b[x] != 0);
        dst[x] = a[x] / b[x];
    }
}
    
    
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
    /*    float val = v3_dot_product(v, n);
    for (int x = 0; x < 3; x++) {
        dst[x] = 2 * val * n[x] - v[x];
    } */
    for(int refIndex = 0; refIndex < 3; refIndex++){
        // Compute b at the start of the loop
        float b = -((n[refIndex])*(v[refIndex])) * (n[refIndex]);
        //Compute Vr by using previously calculated b
        dst[refIndex] = v[refIndex] + (2*b);
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
