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


void v3_angle(float *a, float *b) {
    float A[] = {0.0, 0.0, 0.0};
    float B[] = {0.0, 0.0, 0.0};

    v3_normalize(A, a);
    v3_normalize(B, b);

    return v3_dot_product(A, B);
}


// angle between a and b; no cos-1
void v3_angle_quick(float *a, float *b) {
    float A[] = {0.0, 0.0, 0.0};
    float B[] = {0.0, 0.0, 0.0};
    // inverse cosine x=y
    v3_normalize(A, a);
    v3_normalize(B, b);

    // Check to see if any values are equal to each other, if so pick other
    // values for B
    if(A[0] == B[0] || A[0] == B[1] || A[0] == B[2])
    {
        v3_normalize(B,b);
    }
    if(A[1] == B[0] || A[1] == B[1] || A[1] == B[2])
    {
        v3_normalize(B,b);
    }
    if(A[2] == B[0] || A[2] == B[1] || A[2] == B[2])
    {
        v3_normalize(B,b);
    }

}

void v3_reflect(float *dst, float *v, float *n) {
    float N[] = {0.0, 0.0, 0.0};
    float dotProd = v3_dot_product(v, N);

    v3_normalize(N, n);

    v3_scale(N, 2*dotProd);
    v3_subtract(dst, v, N);

}

float v3_length(float *a){
    return sqrtf((a[0] * a[0]) + (a[1] * a[1]) + (a[2] * a[2]));
}

void v3_normalize(float *dst, float *a) {
    float len = v3_length(a);
    dst[0] = a[0];
    dst[1] = a[1];
    dst[2] = a[2];

    v3_scale(dst, (1/length));

}

bool v3_equals(float *a, float *b, float tolerance) {
    float diff;
    v3_subtract(&diff, a, b);
    if(fabs(diff) > tolerance) {
        return false;
    }
    return true; 
}


int main(int argc, char *argv[]) 
{
    return 0;
}

