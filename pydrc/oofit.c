#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

# include "levmar.h"

// This file accesses the levmar library (which is pre-compiled
// and stored in the lib directory for each major operating system.
// It will create python bindings for the curve fitting code in C

//-----------//
// Equations //
//-----------//

static void drc1(double *p, double *x, int m, int n, void *adata) {
// pass the dose array as a void pointer
// convert the void pointer to a double pointer
double * ptr = (double *) adata;
register int i;
for (i=0; i<n; ++i) {
    // get the value of the thing that the pointer is pointing to as d
    // double d = *ptr;
    double d = *ptr;
    x[i] = 100/(1+(pow(10, (p[0]-d)*p[1])));
    ++ptr; // increment the pointer so it points to the next item in the array
    // this is a really bad/weird way of doing this but I wrote it when I
    // was still learning how to use pointers ... don't judge me
    }
}
static void drc2(double *p, double *x, int m, int n, void *adata) {
double *ptr = (double *) adata;
register int i;
for (i=0; i<n; ++i) {
    double d = *ptr;
    x[i] = p[2] + ((100-p[2])/(1+(pow(10,(p[0]-d)*p[1]))));
    ++ptr;
    }
}
static void drc3(double *p, double *x, int m, int n, void *adata) {
double *ptr = (double *) adata;
register int i;
for (i=0; i<n; ++i) {
    double d = *ptr;
    x[i] = p[2]/(1+(pow(10, (p[0]-d)*p[1])));
    ++ptr;
    }
}
static void drc4(double *p, double *x, int m, int n, void *adata) {
double *ptr = (double *) adata;
register int i;
for (i=0; i <n; ++i) {
    double d = *ptr;
    x[i] = p[2]+((p[3]-p[2])/(1+(pow(10, (p[0]-d)*p[1]))));
    ++ptr;
    }
}

//----------------//
// Misc functions //
//----------------//

static double ec50est(double * res, double * dose, int n /*size*/) {
    // approximates the EC50/IC50 value by finding the 
    // response that is closes to 50%, then returning
    // the corresponding dose. I think this method works
    // well enough for data where finding the EC50 value
    // is reasonably possible
    int i;
    double d50[10]; //arbitrary size
    for (i=0; i<n; ++i) {
      d50[i] = abs(*(res + i) -  50.0);
    }
    double big = 100;
    for(i = 0; i < n; ++i) {
        if(big > d50[i])
        big = d50[i];
    }
    for (i=0; i<n; ++i) {
        if (d50[i] == big)
            return *(dose + i); 
        }
    printf("**Warning: reached 'unreachable' code**\n");
    return 1;
}
static int slope(double * res, int n /*size*/) {
    // used for finding an initial value for the hillslope
    // Basically just tries to figure out if the curve is mostly
    // positive or negative in slope and returns 1 or -1
    int i;
    double inc = 0;
    for (i=0; i<n; i++) {
        // find the differences between each of the points along the curve
        // sum them up
        inc += (*(res + i + 1)) - (*(res + i));
     }
    // if the sum is positive, return 1
    if (inc > 0) {
        return 1;
     }
    // if negative return -1
    if (inc < 0) {
        return -1;
    } else {
        return 1;
     }
}

//-------------------//
// Fitting functions //
//-------------------//

double * fit1 (double * res, double * dose, int n, double *lb, double *ub, double * ret_arr) {
    int m = 2;
    if (n < m) {
        ret_arr[0] = NAN; ret_arr[1] = NAN; ret_arr[2] = NAN;
        ret_arr[3] = NAN; ret_arr[4] = -1; ret_arr[5] = -1;
        return ret_arr;
    }
    // This is the two parameter "contrained" version of fitting.
    // keeps ymin set to 0 and ymax set to 100
    // you can specify your own lower and upper bounds
    double p[2];
    double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
    opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
    opts[4]=LM_DIFF_DELTA; // for finite differences
    p[0] = ec50est(res, dose, n);
    p[1] = slope(res, n);
    int ret = dlevmar_bc_dif(drc1, p, res, m, n, lb, ub, NULL, 1000, opts, info, NULL, NULL, (void *) dose);
    if (ret == -1) {
        ret_arr[0] = NAN; ret_arr[1] = NAN; ret_arr[2] = NAN;
        ret_arr[3] = NAN; ret_arr[4] = 1; ret_arr[5] = 0;
    } else {
        ret_arr[0] = p[0]; ret_arr[1] = p[1]; ret_arr[2] = 0;
        ret_arr[3] = 100; ret_arr[4] = 1; ret_arr[5] = (double) ret;
    }
    return ret_arr;
}
double * fit1d (double * res, double * dose, int n, double * ret_arr) {
    // defualt value version 
    double lb[2] = {-DBL_MAX, -DBL_MAX};
    double ub[2] = {DBL_MAX, DBL_MAX};
    return fit1(res, dose, n, lb, ub, ret_arr);
}
double * fit2 (double * res, double *dose, int n, double *lb, double *ub, double * ret_arr) {
    int m = 3;
    if (n < m) {
        ret_arr[0] = NAN; ret_arr[1] = NAN; ret_arr[2] = NAN;
        ret_arr[3] = NAN; ret_arr[4] = -1; ret_arr[5] = -1;
        return ret_arr;
    }
    double p[3];
    double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
    opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
    opts[4]=LM_DIFF_DELTA; // for finite differences
    p[0] = ec50est(res, dose, n);
    p[1] = slope(res, n);
    p[2] = 0.0;
    int ret = dlevmar_bc_dif(drc2, p, res, m, n, lb, ub, NULL, 1000, opts, info, NULL, NULL, (void *) dose);
    if (ret == -1) {
        ret_arr[0] = NAN; ret_arr[1] = NAN; ret_arr[2] = NAN;
        ret_arr[3] = NAN; ret_arr[4] = 2; ret_arr[5] = 0;
    } else {
        ret_arr[0] = p[0]; ret_arr[1] = p[1]; ret_arr[2] = p[2];
        ret_arr[3] = 100; ret_arr[4] = 2; ret_arr[5] = (double) ret;
    }
    return ret_arr;
}
double * fit2d (double * res, double * dose, int n, double * ret_arr) {
      double lb[3] = {-DBL_MAX, -DBL_MAX, 0};
    double ub[3] = {DBL_MAX, DBL_MAX, DBL_MAX};
    return fit2(res, dose, n, lb, ub, ret_arr);
}
double * fit3 (double * res, double *dose, int n, double *lb, double *ub, double * ret_arr) {
    int m = 3;
    if (n < m) {
        ret_arr[0] = NAN; ret_arr[1] = NAN; ret_arr[2] = NAN;
        ret_arr[3] = NAN; ret_arr[4] = -1; ret_arr[5] = -1;
        return ret_arr;
    }
    double p[3];
    double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
    opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
    opts[4]=LM_DIFF_DELTA; // for finite differences
    p[0] = ec50est(res, dose, n);
    p[1] = slope(res, n);
     p[2] = 100.0;
    int ret = dlevmar_bc_dif(drc3, p, res, m, n, lb, ub, NULL, 1000, opts, info, NULL, NULL, (void *) dose);
    if (ret == -1) {
        ret_arr[0] = NAN; ret_arr[1] = NAN; ret_arr[2] = NAN;
        ret_arr[3] = NAN; ret_arr[4] = 3; ret_arr[5] = 0;
    } else {
        ret_arr[0] = p[0]; ret_arr[1] = p[1]; ret_arr[2] = 0;
        ret_arr[3] = p[2]; ret_arr[4] = 3; ret_arr[5] = (double) ret;
    }
    return ret_arr;
}
double * fit3d (double * res, double * dose, int n, double * ret_arr) {
    double lb[3] = {-DBL_MAX, -DBL_MAX, -DBL_MAX};
    double ub[3] = {DBL_MAX, DBL_MAX, DBL_MAX};
    return fit3(res, dose, n, lb, ub, ret_arr);
}
double * fit4 (double * res, double *dose, int n, double *lb, double *ub, double * ret_arr) {
    int m = 4;
    if (n < m) {
        ret_arr[0] = NAN; ret_arr[1] = NAN; ret_arr[2] = NAN;
        ret_arr[3] = NAN; ret_arr[4] = -1; ret_arr[5] = -1;
        return ret_arr;
    }
    double p[4];
    double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
    opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
    opts[4]=LM_DIFF_DELTA; // for finite differences
    p[0] = ec50est(res, dose, n);
    p[1] = slope(dose, n);
    p[2] = 0.0;
    p[3] = 100.0;
    int ret = dlevmar_bc_dif(drc4, p, res, m, n, lb, ub, NULL, 1000, opts, info, NULL, NULL, (void *) dose);
    if (ret == -1) {
        ret_arr[0] = NAN; ret_arr[1] = NAN; ret_arr[2] = NAN;
        ret_arr[3] = NAN; ret_arr[4] = 4; ret_arr[5] = 0;
    } else {
        ret_arr[0] = p[0]; ret_arr[1] = p[1]; ret_arr[2] = p[2];
        ret_arr[3] = p[3]; ret_arr[4] = 4; ret_arr[5] = (double) ret;
    }
    return ret_arr;

}
double * fit4d (double * res, double * dose, int n, double * ret_arr) {
    double lb[4] = {-DBL_MAX, -DBL_MAX, 0, -DBL_MAX};
    double ub[4] = {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX};
    return fit4(res, dose, n, lb, ub, ret_arr);
}
void pfit (double * ret_arr) {
    printf("--EasyFit---\nEC50      : %.7g\nhillslope : %.7g\nymin      : %.7g\nymax      : %.7g\nequation  : %.7g\nconv      : %.7g\n",
		  ret_arr[0], ret_arr[1], ret_arr[2], ret_arr[3], ret_arr[4], ret_arr[5]);
}