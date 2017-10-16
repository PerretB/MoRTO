MoRTO Monotonic regression with a tree ordering
===============================================

Description
-----------

MoRTO is a single function library to compute a least square 
monotonic / isotonic regression with a tree ordering.

Given a vector of observed data, a vector of weights, and a tree ordering on 
the observed data points, the least square isotonic / monotonic regression finds 
the vector results such that:

    result = argmin_x sum_i (weights[i] * (data[i]-x[i])²)
        s.t. for all i, j, j is an ancestor of i implies x[i] <= x[j]



MoRTO is written in C++ and provides Python/Numpy bindings.



Example
-------
Given the following tree where each node is an ordered pair 
(node index, node value (input data)).
                                    
                         (7,10)
                           ^
                           |
                 +-------------------+
                 |         |         |
                 +         +         +
              (4,11)     (5,7)     (6,5)
                ^                    ^
                |                    |
           +----+----+           +---+---+
           |         |           |       |
           +         +           +       +
        (0,13)    (1,14)       (2,6)   (3,8)
    
The least square monotonic regression of the node values with respect to the 
ordering given by the parent relation (node i is before j if j is an ancestor 
of i) can be computed in C++:
 
    #include <vector>
    #include <morto.h>
    
    vector<size_t> parents = {4, 4, 6, 6, 7, 7, 7, 7};
    vector<double> values = {13, 14, 6, 8, 11, 7, 2, 10};
    
    vector<double> results = morto::monotonicRegressionOnTree(parents, 
                                                              values)
    // results = {12, 12, 6, 6.5, 12, 7, 6.5, 12}
     

Or in Python:

    import numpy as np
    import morto
    
    parents = np.asarray([4, 4, 6, 6, 7, 7, 7, 7], dtype=np.int32)
    values = np.asarray([13, 14, 6, 8, 11, 7, 5, 10], dtype=np.float64)
    results = np.zeros((8,), dtype=np.float64)
    
    morto.monotonicRegressionOnTree(parents, values, results)
    # results = [12, 12, 6, 6.5, 12, 7, 6.5, 12]


Simple use
----------

A simple use in a C++ project does not require any installation: 
simply drop the 2 source files in your project.

    include/morto.h
    src/morto.cpp
    
There is a single dependency to **Boost Heap**.    
morto.cpp requires a **C++ 14 compiler**.  

Installation
------------

In a more complex scenario the included cmake scripts allows to build and 
install MoRTO as a shared library with Python bindings.

### Requirements
* **C++ 14 compiler**
* **cmake**
* **Boost Heap**
* Boost Test Library (optional to run unit tests)
* Python 2.7+ (optional for Python bindings)
* numpy (optional for Python bindings)
* Swig (optional for Python bindings)
* pip (optional to install Python bindings)
* ccmake (optional for convenience)
* git (optional for convenience)

### Procedure

In your favorite terminal:

    git clone https://github.com/PerretB/MoRTO.git
    cd MoRTO
    mkdir build
    cd build
    ccmake ..

Then press 'c'. Modify options to fit your needs. 
Press 'c' again, and finally press 'g'. 
Everything is ready to build and install the library: back to the terminal,

     make
     make install

### Usage

In C++: 

    #include <morto.h>

And compile with -lMoRTO

In Python:

    import morto
    
Et voilà !

Documentation
-------------

The single function is described in the header file using doxygen syntax or in 
the python module using docstring format.


Algorithm
---------

Time complexity is in O(n*log(n)) (with n the size of the vector data).
Space complexity is in O(n).

The algorithm is described in Pardalos, P. & Xue, G. Algorithms for a Class of 
Isotonic Regression Problems. Algorithmica (1999) 23: 211. 
doi:10.1007/PL00009258


License
-------

This library is distributed under the CeCILL-B license http://www.cecill.info.

CeCILL-B is similar to BSD or Apache license with an obligation of citation: 
you can freely use, modify, and distribute this library in any software if you 
provide a correct citation.

Author and contact information
------------------------------

Benjamin Perret

benjamin.perret@esiee.fr

ESIEE Paris 2017