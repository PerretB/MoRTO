MoRTO Monotonic regression with a tree ordering
===============================================

Description
-----------

MoRTO is a single function library to compute a least square monotonic / isotonic regression with a tree ordering.

Given a vector of observed data, a vector of weights, and a tree ordering on the observed data points,
the least square isotonic / monotonic regression finds the vector results such that:
	result = argmin_x sum_i (weights[i] * (data[i]-x[i])²)
	    s.t. for all i, j, rank(i) <= rank(j) implies x[i] <= x[j]

The result is obtained with a complexity in O(n*log(n)) (with n the size of the vector data) thanks to the algorithm
described in Pardalos, P. & Xue, G. Algorithms for a Class of Isotonic Regression Problems.
Algorithmica (1999) 23: 211. doi:10.1007/PL00009258


License
-------

This library is distributed under the CeCILL-B license http://www.cecill.info.

CeCILL-B is similar to BSD or Apache license with an obligation of citation: you can freely use, modify,
and distribute this library in any software if you provide a correct citation.

Author and contact information
------------------------------

Benjamin Perret
benjamin.perret@esiee.fr
ESIEE Paris 2017
