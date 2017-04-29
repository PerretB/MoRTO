#include "morto.h"
#include <iostream>

using namespace morto;
using namespace std;


template<typename T>
ostream & operator<<(ostream & o, const vector<T> & vec)
{
    o << "{";
    for (size_t i = 0; i < vec.size() - 1; ++i)
        o << vec[i] << ", ";
    if (!vec.empty())
        o << vec.back();
    o << "}";
    return o;
}

void test()
{
    vector<size_t> parents = { 4,4,6,6,7,7,7,7 };
    //vector<double> values = { 0,1,2,3,4,5,6,7 };
    vector<double> values = { 13,14,6,8,11,7,5,10 };
    vector<double> result = monotonicRegressionOnTree(parents, values);
    cout << result << endl;
}


int main(int argc, char ** argv)
{
    test();
    getchar();
    return 0;
}

