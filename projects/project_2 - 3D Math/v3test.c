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

float scale_vals[4] = {0.0, 4.5, 0000.7, -2.008};

//v3 from points answers
//TODO
float point_test[4][3] = { {},
                           {},
                           {},
                           {} };

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
//TODO
float cross_test[4][3] = { {},
                           {},
                           {},
                           {} };

//v3 scale product answers
//TODO
float scale_test[4][3] = { {},
                           {},
                           {},
                           {} };

//v3 angle answers
//TODO
float angle_test[4] = {0.0, 0.0, 0.0, 0.0};

//v3 angle quick answers
//TODO
float qangle_test[4] = {0.0, 0.0, 0.0, 0.0};

//v3 reflect product answers
//TODO
float reflect_test[4][3] = { {4.14, 0.8, -3.21},
                             {-7.75, 19.8, 0.8},
                             {2.895, -0.1, -0.01},
                             {-8.995, 18.9, 4} };

//v3 normalize answers
float norm_test[4][3] = { {0.78122, 0.15096, -0.605728},
                          {-0.364423, 0.93055, 0.037598},
                          {0.999398, -0.03452115, -0.003445215},
                          {-0.44221, 0.886904, 0.187704} };

//v3 length answers
float length_test[4] = {5.29914, 21.277, 2.89674, 21.3101};




bool is_empty( float *arr) {
    return arr[0] == 0.0 && arr[0] == 0.0 && arr[0] == 0.0;
}


void print_pass (int fails, char* msg) {
    if(fails == 0) {
       printf("\nAll Tests Passed For: %s\n\n", msg);
    }
    else {
       printf("\n%d Test(s) Failed For: %s\n\n", fails, msg);
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
                   act_float = dot_test[curNum-x];
                   pred_float = v3_dot_product(a, b);
                   break;

               case CROSS:
                   set_vector(act_vec, cross_test[curNum-x]);
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
                   act_float = angle_test[x];
                   pred_float = v3_angle(a, b);
                   break;

               case QANGLE:        
                   act_float = qangle_test[x];
                   pred_float = v3_angle_quick(a, b);
                   break;

               case REFLCT:
                   set_vector(act_vec, reflect_test[curNum]);
                   v3_reflect(pred_vec, a, b);
                   break;

              case NORM:
                   set_vector(act_vec, norm_test[x]);
                   v3_normalize(pred_vec, a);
                   break;

              case LEN:
                   if(loops == 0) {
                       act_float = length_test[curNum];
                       pred_float = v3_length(a);
                   }
                   else {
                       act_float = length_test[curNum];
                       pred_float = v3_length(b);
                   }
                   break;
            }  
            if (!is_empty(pred_vec) && !is_empty(act_vec)) {
                if (!v3_equals(act_vec, pred_vec, 0.0001)) {
                    fails ++;
                    fprintf(stdout, "\tTest Failed: %s, test #%d\n", method, curNum+1);
                    fprintf(stdout, "\t\tActual: %f, %f, %f\n",act_vec[0], act_vec[1], act_vec[2]); 
                    fprintf(stdout, "\t\tPredicted: %f, %f, %f\n\n",pred_vec[0], pred_vec[1], pred_vec[2]); 
                }
            }  
            else if (fabs(act_float-pred_float) > 0.0001) {
                fails ++;
                fprintf(stdout, "\tTest Failed: %s, test #%d\n", method, curNum+1);
                fprintf(stdout, "\t\tActual: %f\n",act_float); 
                fprintf(stdout, "\t\tPredicted: %f\n\n",pred_float); 
            }
        } 
        loops ++; 
    }
    print_pass(fails, method);
}


int main(int argc, char *argv[]) {

     //void_test(POINT,"v3_from_points");
    void_test(ADD,"v3_add");
    void_test(SUB,"v3_subtract");
    void_test(DOT,"v3_dot_product");
    //void_test(CROSS,"v3_cross_product");
    void_test(SCALE,"v3_scale");
    //void_test(ANGLE,"v3_angle");
    //void_test(QANGLE,"v3_angle_quick");
    //void_test(REFLCT,"v3_reflect");
    void_test(NORM,"v3_normalize");
    void_test(LEN,"v3_length");
    return 0;
}

