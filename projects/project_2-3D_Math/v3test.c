#include "v3math.h"
#define VAL_NUMS 2
#define POINT  999
#define ADD    998
#define SUB    997
#define DOT    996
#define CROSS  995
#define SCALE  994
#define ANGLE  993
#define QANGLE 992
#define REFLCT 991
#define NORM   990
#define LEN    989

// all values for a
float a_vals[2][3] = { {1.25, 0.80, -3.2},
                       {.005, -0.1, 0.00} };
//all values for b
float b_vals[2][3] = { {2.890, 0.00, -0.01},
                       {-9.00, 19.0, 4.000} };

//values for scales
float scale_vals[4] = {-0.1, 4.5, 0.0007, -2.008};

//v3 from points answers
float point_test[4][3] = { {1.64, -0.8, 3.19},
                           {-10.25, 18.2, 7.2},
                           {2.885, 0.1, -0.01},
                           {-9.005, 19.1, 4.0} };

//v3 add answers
float add_test[4][3] = { {4.14, 0.8, -3.21},
                         {-7.75, 19.8, 0.8},
                         {2.895, -0.1, -0.01},
                         {-8.995, 18.9, 4} };

//v3 subtract answers
float sub_test[4][3] = { {-1.64, 0.8, -3.19},
                         {10.25, -18.2, -7.2},
                         {-2.885, -0.1, 0.01},
                         {9.005, -19.1, -4} };

//v3 dot product answers
float dot_test[4] = {3.6445, -8.85, 0.01445, -1.945};

//v3 cross product answers
float cross_test[4][3] = { {-0.008, -9.2355, -2.312},
                           {64.0, 23.8, 30.95},
                           {0.001, 0.00005, 0.289},
                           {-0.4, -0.02, -0.805} };

//v3 scale product answers
float scale_test[4][3] = { {-0.125, -0.08, .320},
                           {0.0225, -0.45, 0.0},
                           {0.002023, 0.0, -0.000007},
                           {18.071999, -38.151997, -8.032} };

//v3 angle answers
float angle_test[4] = {1.20519998, 1.68828995, 1.52083823, 2.70857798};

//v3 angle quick answers
float qangle_test[4] = {0.3575062899839984, -0.117223484322137562, 
                        0.049937317994116014, -0.9077048919330981};

//v3 reflect product answers
float reflect_test[4][3] = { {-19.630251, 0.800000, -3.199360},
                             {-201.250000, -576.799988, 99.200005},
                             {-0.078521, -0.100000, 0.000000},
                             {-0.805000, 72.099998, 0.000000} };

//v3 normalize answers
float norm_test[4][3] = { {0.354369, 0.226796, -0.907186},
                          {0.0499376, -0.998752, 0.0},
                          {0.999994, 0.0, -0.00346019},
                          {-0.420542, 0.887812, 0.186908} };

//v3 length answers
float length_test[4] = {3.52739, 0.100125, 2.89002, 21.4009};




bool is_empty( float *arr) {
    return arr[0] == 0.0 && arr[0] == 0.0 && arr[0] == 0.0;
}


void print_pass (int fails, char* msg) {
    if(fails == 0) {
       printf("\nAll Tests Passed For: %s\n", msg);
    }
    else {
       printf("\n%d Test(s) Failed For: %s\n", fails, msg);
    }
}


void set_vector (float *dst, float *val) {
    dst[0] = val[0];
    dst[1] = val[1];
    dst[2] = val[2];
}


void void_test(int type, char* method) {
    float act_vec[3] = {0.0, 0.0, 0.0};
    float pred_vec[3] = {0.0, 0.0, 0.0};
    float act_float = 0.0, pred_float = 0.0;
    int fails = 0, loops = 0, curNum;
    for (int x = 0; x < VAL_NUMS; x++) {
        float a[3] = {a_vals[x][0], a_vals[x][1], a_vals[x][2]};

        for (int y = 0; y < VAL_NUMS; y++) {
            float b[3] = {b_vals[y][0], b_vals[y][1], b_vals[y][2]};
            curNum = x + y + loops;
            switch(type) {

               case POINT:
                  set_vector(act_vec, point_test[curNum]);
                  v3_from_points(pred_vec, a, b);
                  break;

               case ADD:
                   set_vector(act_vec, add_test[curNum]);
                   v3_add(pred_vec, a, b);
                   break;

               case SUB:
                   set_vector(act_vec, sub_test[curNum]);
                   v3_subtract(pred_vec, a, b);
                   break;

               case DOT:
                   act_float = dot_test[curNum];
                   pred_float = v3_dot_product(a, b);
                   break;

               case CROSS:
                   set_vector(act_vec, cross_test[curNum]);
                   v3_cross_product(pred_vec, a, b);
                   break;

               case SCALE:
                   if (loops == 0) {
                      set_vector(pred_vec, a_vals[y]);
                   }
                   else {
                      set_vector(pred_vec, b_vals[y]);
                   }
                   set_vector(act_vec, scale_test[curNum]);
                   v3_scale(pred_vec, scale_vals[curNum]);
                   break;

               case ANGLE:
                   act_float = angle_test[curNum];
                   pred_float = v3_angle(a, b);

                   break;

               case QANGLE:
                   act_float = qangle_test[curNum];
                   pred_float = v3_angle_quick(a, b);
                   break;

               case REFLCT:
                   set_vector(act_vec, reflect_test[curNum]);
                   v3_reflect(pred_vec, a, b);
                   break;

              case NORM:
                   set_vector(act_vec, norm_test[curNum]);
                   if(loops == 0) {
                       float a[3] = {a_vals[y][0], a_vals[y][1], a_vals[y][2]};
                       v3_normalize(pred_vec, a);
                   }
                   else {
                       float b[3] = {b_vals[y][0], b_vals[y][1], b_vals[y][2]};
                       v3_normalize(pred_vec, b);
                   }
                   break;

              case LEN:
                   if(loops == 0) {
                       float a[3] = {a_vals[y][0], a_vals[y][1], a_vals[y][2]};
                       pred_float = v3_length(a);
                   }
                   else {
                       float b[3] = {b_vals[y][0], b_vals[y][1], b_vals[y][2]};
                       pred_float = v3_length(b);
                   }
                   act_float = length_test[curNum];
                   break;
            }
            if (!is_empty(pred_vec) && !is_empty(act_vec) && !v3_equals(act_vec, pred_vec, 0.001)) {
                    fails ++;
                    fprintf(stdout, "\n\tTest Failed: %s, test #%d\n", method, curNum+1);
                    fprintf(stdout, "\t\tActual: %f, %f, %f\n",act_vec[0], act_vec[1], act_vec[2]);
                    fprintf(stdout, "\t\tPredicted: %f, %f, %f\n",pred_vec[0], pred_vec[1], pred_vec[2]);
            }
            else if (fabs(act_float-pred_float) > 0.001) {
                fails ++;
                fprintf(stdout, "\n\tTest Failed: %s, test #%d\n", method, curNum+1);
                fprintf(stdout, "\t\tActual: %f\n",act_float);
                fprintf(stdout, "\t\tPredicted: %f\n",pred_float);
            }
        }
        loops ++;
    }
    print_pass(fails, method);
}


int main(int argc, char *argv[]) {

    void_test(POINT,"v3_from_points");
    void_test(ADD,"v3_add");
    void_test(SUB,"v3_subtract");
    void_test(DOT,"v3_dot_product");
    void_test(CROSS,"v3_cross_product");
    void_test(SCALE,"v3_scale");
    void_test(ANGLE,"v3_angle");
    void_test(QANGLE,"v3_angle_quick");
    void_test(REFLCT,"v3_reflect");
    void_test(NORM,"v3_normalize");
    void_test(LEN,"v3_length");
    return 0;
}
