#pragma once
#include <morto.h>


/**
	 * Wrapper on the library function
	 * @param nbNodes
	 * @param parents
	 * @param values
	 * @param result
	 * @param weights
	 */
void monotonicRegressionOnTree(
        int * parents, int sizeParents,
        double * values, int sizeValues,
        double * results, int sizeResults,
        double * weights=nullptr, int sizeWeights=0
        );

