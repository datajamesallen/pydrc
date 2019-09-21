/* File: oofit.i */
%module oofit
%{
    #define SWIG_FILE_WITH_INIT
    #include "oofit.h"
%}
double * fit1(double * res, double * dose, int n, double *lb, double *ub, double * ret_arr);
