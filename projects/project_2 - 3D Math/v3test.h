#ifndef V3TEST_H_INCLUDE
#define V3TEST_H_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "v3math.c"

void v3_from_points_test (float *a, float *b);

void v3_add_test (float *a, float *b);

void v3_subtract_test (float *a, float *b);

void v3_dot_product_test (float *a, float *b);

void v3_cross_product_test (float *a, float *b);

void v3_scale_test (float s);

void v3_angle_test (float *a, float *b);

void v3_angle_quick_test (float *a, float *b);

void v3_reflect_test (float *v, float *n);

void v3_length_test (float *a);

void v3_normalize_test (float *a);

int mainTest (int argc, char *argv[]);

#endif
