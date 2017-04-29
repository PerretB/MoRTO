/*
Copyright Benjamin Perret ESIEE Paris 2017

benjamin.perret@esiee.fr

This software is a computer program whose purpose is to compute isotonic/
monotonic regression on tree ordering.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
 */

#pragma once

#include <vector>
#include <string>

namespace morto
{
	/**
	* \brief Performs a least square monotonic / isotonic regression with respect to a tree ordering.
	* 
	* \param parents Parent relation between nodes.
	* The indexing of the nodes are assumed to be in topological order (e.g. nodes indices correspond to a post-order traversal of tree).
	* Note that there may be several roots (i.e. the ordering is a forest)
	* Preconditions :
	*		\li parents.size() > 0
	*		\li for all i 0<=parents[i]<parents.size()
	*		\li "node i is the parent of node j" <=> parents[j] == i
	*		\li "node i is the root" <=> parents[i] == i
	*		\li nodes are in topological order : parents[i] >= i
	* \param values Input data associated to nodes.
	* The observed value at node i is values[i].
	* Preconditions :
	*		\li values.size() == parents.size()
	* \param weights Weights associated to nodes.
	* If weights.size() == 0 , every node is assumed to have the same weight.
	* Else, the weight of node i is weights[i].
	* Preconditions :
	*		\li weights.size() == parents.size() || weights.empty()
	 *		\li for all i weights[i]>0
	* \return vector<double> results containing the isotonic regression values for each node.
	* The regression value at node i is result[i].
	* Postconditions :
	*		\li results.size() == parents.size()
	*		\li for all i, results[i] <= results[parents[i]]
	*
	* The isotonic / monotonic regression solves the following problem:
	*	result = argmin_x sum_i (weights[i] * (values[i]-x[i])²)
	*		s.t. for all i, x[i] <= x[parent[i]]
	*
	* The algorithm runs in O(n*log(n)) with n the number of nodes (size of the parents vector).
	* The algorithm is an implementation of: 
	*	Pardalos, P. & Xue, G. Algorithms for a Class of Isotonic Regression Problems. Algorithmica (1999) 23: 211. doi:10.1007/PL00009258
	*		
	* Example:
	* Given the	following tree where each node is a triplet (node index, node value, node weight).
	*								
	*				   (7,10,1)
	*					   ^
	*					   |
	*			 +-------------------+
	*			 |         |         |
	*			 +         +         +
	*		 (4,11,1)   (5,7,1)   (6,5,1)
	*			^                    ^
	*			|                    |
	*	   +----+----+           +---+---+
	*	   |         |           |       |
	*	   +         +           +       +
	*	(0,13,1)  (1,14,1)    (2,6,1) (3,8,1)
	*
	* 
	* The inputs of the method should be:
	*  \li parents = {4, 4, 6, 6, 7, 7, 7, 7}
	*  \li values = {13, 14, 6, 8, 11, 7, 2, 10}
	*  \li weights = {1, 1, 1, 1, 1, 1, 1, 1} or {}
	* The expected output is {12, 12, 6, 6.5, 12, 7, 6.5, 12}
	* 
	*/
	std::vector<double> monotonicRegressionOnTree(
		const std::vector<size_t> & parents, 
		const std::vector<double> & values, 
		const std::vector<double> & weights = {});


	
	/**
	 * \brief A simple holder for the exceptions thrown by the library.
	 */
	struct MortoException
	{
		/**
		 * \brief Description of the cause of the exception.
		 */
		std::string msg;
	};

}