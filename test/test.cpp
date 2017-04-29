
#define BOOST_TEST_MODULE MoRTO_Test
#include <boost/test/unit_test.hpp>
#include "morto.h"
#include <vector>

using namespace std;
using namespace morto;


BOOST_AUTO_TEST_CASE(DieOnEmpty)
{
	vector<size_t> parents = {  };
	vector<double> values = {  };

	BOOST_REQUIRE_THROW(monotonicRegressionOnTree(parents, values), MortoException);
}

/*
BOOST_AUTO_TEST_CASE(MoRTO_Test)
{
	vector<size_t> parents = { 4,4,6,6,7,7,7,7 };
	//vector<double> values = { 0,1,2,3,4,5,6,7 };
	vector<double> values = { 13,14,6,8,11,7,5,10 };
	vector<double> result = monotonicRegressionOnTree(parents, values);


	BOOST_CHECK(test_object.is_valid());
}*/


