#ifndef V3MATH_H_INCLUDE
#define V3MATH_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void v3_from_points (float *dst, float *a, float *b);

void v3_add (float *dst, float *a, float *b);

void v3_subtract (float *dst, float *a, float *b);

float v3_dot_product (float *a, float *b);

void v3_cross_product (float *dst, float *a, float *b);

void v3_scale (float *dst, float s);

void v3_angle (float *dst, float *a, float *b);

void v3_angle_quick (float *dst, float *a, float *b);

void v3_reflect (float *dst, float *v, float *n);

float v3_length (float *a);

void v3_normalize(float *dst, float *a);

bool v3_equals (float *a, float *b, float tolerance);

int main (int argc, char *argv[]);

#endif
