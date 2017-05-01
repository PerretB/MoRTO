%module morto

%{
#define SWIG_FILE_WITH_INIT
#include "morto_python.h"
static PyObject* pMortoException;
%}

%include "numpy.i"
%init %{
import_array();
pMortoException = PyErr_NewException((char*)"_morto.MortoException", NULL, NULL);
Py_INCREF(pMortoException);
PyModule_AddObject(m, "MortoException", pMortoException);
%}

%pythoncode %{
MortoException = _morto.MortoException
              %}

%exception monotonicRegressionOnTreeWeighted {
        try {
            $action
        } catch (morto::MortoException &e) {
            PyErr_SetString(pMortoException, e.msg.c_str());
            return NULL;
        }
}

%exception monotonicRegressionOnTree {
        try {
            $action
        } catch (morto::MortoException &e) {
            PyErr_SetString(pMortoException, e.msg.c_str());
            return NULL;
        }
}

%apply (int* IN_ARRAY1, int DIM1) {(int*parents, int sizeParents)};
%apply (double* IN_ARRAY1, int DIM1) {(double*values, int sizeValues)};
%apply (double* IN_ARRAY1, int DIM1) {(double*weights, int sizeWeights)};
%apply (double* INPLACE_ARRAY1, int DIM1 ) {(double *results, int sizeResults)};
void monotonicRegressionOnTreeWeighted(
        int * parents, int sizeParents,
        double * values, int sizeValues,
        double * weights, int sizeWeights,
        double * results, int sizeResults
);

%apply (int* IN_ARRAY1, int DIM1) {(int*parents, int sizeParents)};
%apply (double* IN_ARRAY1, int DIM1) {(double*values, int sizeValues)};
%apply (double* INPLACE_ARRAY1, int DIM1 ) {(double *results, int sizeResults)};
void monotonicRegressionOnTree(
        int * parents, int sizeParents,
        double * values, int sizeValues,
        double * results, int sizeResults
);