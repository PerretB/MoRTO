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

%define DOCSTRING_MODULE
"Monotonic / isotonic regression with respect to a tree ordering.
Author: Benjamin Perret
Year: 2017"
%enddef
%module(docstring=DOCSTRING_MODULE) morto

%exception monotonicRegressionOnTree {
        try {
            $action
        } catch (morto::MortoException &e) {
            PyErr_SetString(pMortoException, e.msg.c_str());
            return NULL;
        }
}

%define DOCSTRING_METHOD
"Performs a least square monotonic / isotonic regression with respect to a tree ordering.

:param parents (numpy array of dtype int32 of shape (n,))
Parent relation between nodes.
The indexing of the nodes are assumed to be in topological order (e.g. nodes indices correspond to a post-order traversal of tree).
Note that there may be several roots (i.e. the ordering is a forest)
Preconditions :
	- parents.size() > 0
	- for all i 0<=parents[i]<parents.size()
	- 'node i is the parent of node j' <=> parents[j] == i
	- 'node i is the root' <=> parents[i] == i
	- nodes are in topological order : parents[i] >= i

:param values (numpy array of dtype float64 of shape (n,))
Input data associated to nodes.
The observed value at node i is values[i].

:param  results (numpy array of dtype float64 of shape (n,))
Array to store the result of the isotonic regression values for each node.
The regression value at node i is result[i].
Postconditions :
	- for all i, results[i] <= results[parents[i]]

:param weights optional (numpy array of dtype float64 of shape (n,))
Weights associated to nodes.
If no weights are given, every node is assumed to have the same weight.
Else, the weight of node i is weights[i].
Preconditions :
	- for all i weights[i]>0

The isotonic / monotonic regression solves the following problem:
result = argmin_x sum_i (weights[i] * (values[i]-x[i])^2)
	s.t. for all i, x[i] <= x[parent[i]]

The algorithm runs in O(n*log(n)) with n the number of nodes (size of the parents vector).

The algorithm is an implementation of:
Pardalos, P. & Xue, G. Algorithms for a Class of Isotonic Regression Problems. Algorithmica (1999) 23: 211. doi:10.1007/PL00009258

Example:
Given the	following tree where each node is a triplet (node index, node value, node weight).

			   (7,10,1)
				   ^
				   |
		 +-------------------+
		 |         |         |
		 +         +         +
	 (4,11,1)   (5,7,1)   (6,5,1)
		^                    ^
		|                    |
   +----+----+           +---+---+
   |         |           |       |
   +         +           +       +
(0,13,1)  (1,14,1)    (2,6,1) (3,8,1)

The inputs of the method should be:
 - parents = np.asarray([4, 4, 6, 6, 7, 7, 7, 7], dtype=int32)
 - values = np.asarray([13, 14, 6, 8, 11, 7, 2, 10], dtype=np.float64)
 - results = np.zeros(parents.shape, dtype=np.float64)
 - weights = np.asarray([1, 1, 1, 1, 1, 1, 1, 1], dtype=np.float64) or nothing
The expected output (stored in result array) is [12, 12, 6, 6.5, 12, 7, 6.5, 12]
"
%enddef


%apply (int* IN_ARRAY1, int DIM1) {(int*parents, int sizeParents)};
%apply (double* IN_ARRAY1, int DIM1) {(double*values, int sizeValues)};
%apply (double* INPLACE_ARRAY1, int DIM1 ) {(double *results, int sizeResults)};
%apply (double* IN_ARRAY1, int DIM1) {(double*weights, int sizeWeights)};
%feature("autodoc", DOCSTRING_METHOD) monotonicRegressionOnTree;
void monotonicRegressionOnTree(
        int * parents, int sizeParents,
        double * values, int sizeValues,
        double * results, int sizeResults,
        double * weights=nullptr, int sizeWeights=0
);

