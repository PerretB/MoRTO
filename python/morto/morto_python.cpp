#include "morto_python.h"
#include "morto.h"
#include <vector>

using namespace morto;
using namespace std;

void monotonicRegressionOnTree(
        int * parents, int sizeParents,
        double * values, int sizeValues,
        double * results, int sizeResults,
        double * weights, int sizeWeights)
{
    if(sizeResults!=sizeParents)
    {
        throw MortoException{"Size of output array is incorrect."};
    }
    vector<size_t> v_parents;
    v_parents.assign(parents, parents+sizeParents);

    vector<double> v_values;
    v_values.assign(values, values+sizeValues);

    vector<double> v_weights;
    if(weights!= nullptr)
    {
        v_weights.assign(weights, weights+sizeWeights);
    }

    vector<double> v_results = monotonicRegressionOnTree(v_parents,v_values,v_weights);

    for(int i=0; i<sizeParents; ++i)
        results[i] = v_results[i];

}


