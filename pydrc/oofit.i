/* File: oofit.i */
%module oofit
%{
    #define SWIG_FILE_WITH_INIT
    #include "oofit.h"
%}
%include "numpy.i"

%init %{
    import_array();
%}

%apply(double * IN_ARRAY1, int DIM1) {(double * res, int n)};
%apply(double * IN_ARRAY1, int DIM1) {(double * dose, int n)};
%apply(double * ARGOUT_ARRAY1) {(double * ret_arr)};
double * fit1(double * res, double * dose, int n, double *lb, double *ub, double * ret_arr);
