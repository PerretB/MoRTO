
#define BOOST_TEST_MODULE MoRTO_Test

#include <boost/test/unit_test.hpp>
#include "morto.h"
#include <vector>

using namespace std;
using namespace morto;

BOOST_AUTO_TEST_SUITE(incorrect_input);

BOOST_AUTO_TEST_CASE(DieOnEmpty)
{
	vector<size_t> parents = {  };
	vector<double> values = {  };

	BOOST_REQUIRE_THROW(monotonicRegressionOnTree(parents, values), MortoException);
}

BOOST_AUTO_TEST_CASE(DieOnOutOfDomain)
{
    vector<size_t> parents = {0 , 3};
    vector<double> values = {0, 0};

    BOOST_REQUIRE_THROW(monotonicRegressionOnTree(parents, values), MortoException);
}

BOOST_AUTO_TEST_CASE(DieOnWrongSizeValues)
{
    vector<size_t> parents = {0 , 1};
    vector<double> values = {0};

    BOOST_REQUIRE_THROW(monotonicRegressionOnTree(parents, values), MortoException);
}

BOOST_AUTO_TEST_CASE(DieOnNotTopologicalOrder)
{
    vector<size_t> parents = {1 , 0};
    vector<double> values = {0, 0};

    BOOST_REQUIRE_THROW(monotonicRegressionOnTree(parents, values), MortoException);
}

BOOST_AUTO_TEST_CASE(DieOnWrongSizeWeights)
{
    vector<size_t> parents = {0 , 1};
    vector<double> values = {0, 0};
    vector<double> weights = {0};
    BOOST_REQUIRE_THROW(monotonicRegressionOnTree(parents, values, weights), MortoException);
}

BOOST_AUTO_TEST_CASE(DieOnNegativeWeight)
{
    vector<size_t> parents = {0 , 1};
    vector<double> values = {0, 0};
    vector<double> weights = {1, -2};
    BOOST_REQUIRE_THROW(monotonicRegressionOnTree(parents, values, weights), MortoException);
}

BOOST_AUTO_TEST_CASE(DieOnZeroWeight)
{
    vector<size_t> parents = {0 , 1};
    vector<double> values = {0, 0};
    vector<double> weights = {1, 0};
    BOOST_REQUIRE_THROW(monotonicRegressionOnTree(parents, values, weights), MortoException);
}

BOOST_AUTO_TEST_SUITE_END();

bool testQuasiEquality(vector<double> v1, vector<double> v2)
{
    if (v1.size()!=v2.size())
        return false;
    for(int i=0; i<v1.size(); ++i)
        if(abs(v1[i]-v2[i])>1E-8)
            return false;
    return true;
}

BOOST_AUTO_TEST_SUITE(valid_input);

BOOST_AUTO_TEST_CASE(IdempotentOnIncreasingValues)
{
    vector<size_t> parents = { 4,4,6,6,7,7,7,7 };
    vector<double> values = { 0,1,2,3,4,5,6,7 };
    vector<double> result = monotonicRegressionOnTree(parents, values);

    BOOST_CHECK(testQuasiEquality(values,result));
}

BOOST_AUTO_TEST_CASE(IdempotentOnIncreasingValuesWithWeights)
{
    vector<size_t> parents = { 4,4,6,6,7,7,7,7 };
    vector<double> values = { 0,1,2,3,4,5,6,7 };
    vector<double> weights = { 1,2,3,4,5,6,7,8 };
    vector<double> result = monotonicRegressionOnTree(parents, values, weights);

    BOOST_CHECK(testQuasiEquality(values,result));
}

BOOST_AUTO_TEST_CASE(IdempotentOnIncreasingValuesWithWeightsOnForest)
{
    vector<size_t> parents = { 4,4,6,6,4,5,6 };
    vector<double> values = { 0,1,2,3,4,5,6 };
    vector<double> weights = { 1,2,3,4,5,6,7 };
    vector<double> result = monotonicRegressionOnTree(parents, values, weights);

    BOOST_CHECK(testQuasiEquality(values,result));
}

BOOST_AUTO_TEST_CASE(NormalNoWeights)
{
    vector<size_t> parents = { 4,4,6,6,7,7,7,7 };
    vector<double> values = { 13,14,6,8,11,7,5,10 };

    vector<double> result = monotonicRegressionOnTree(parents, values);

    BOOST_CHECK(testQuasiEquality({12, 12, 6, 6.5, 12, 7, 6.5, 12},result));
}

BOOST_AUTO_TEST_CASE(NormalWeights)
{
    vector<size_t> parents = { 4,4,6,6,7,7,7,7 };
    vector<double> values = { 13,14,6,8,11,7,5,10 };
    vector<double> weights = { 1,1,1,1,1,1,2,1 };
    vector<double> result = monotonicRegressionOnTree(parents, values, weights);

    BOOST_CHECK(testQuasiEquality({12, 12, 6, 6, 12, 7, 6, 12},result));
}

BOOST_AUTO_TEST_CASE(NormalWeightsForest)
{
    vector<size_t> parents = { 4,4,6,6,4,5,6 };
    vector<double> values = { 14,14,6,8,11,7,5};
    vector<double> weights = { 1,1,1,1,1,1,2 };
    vector<double> result = monotonicRegressionOnTree(parents, values, weights);

    BOOST_CHECK(testQuasiEquality({13, 13, 6, 6, 13, 7, 6},result));
}

BOOST_AUTO_TEST_SUITE_END();
/*
BOOST_AUTO_TEST_CASE(MoRTO_Test)
{
	vector<size_t> parents = { 4,4,6,6,7,7,7,7 };
	//vector<double> values = { 0,1,2,3,4,5,6,7 };
	vector<double> values = { 13,14,6,8,11,7,5,10 };
	vector<double> result = monotonicRegressionOnTree(parents, values);


	BOOST_CHECK(test_object.is_valid());
}*/


