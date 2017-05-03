/*
Copyright Benjamin Perret ESIEE Paris 2017

benjamin.perret@esiee.fr

This software is a computer program whose purpose is to compute isotonic/
monotonic regression on tree ordering.

This software is governed by the CeCILL-B license under French law and
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
knowledge of the CeCILL-B license and that you accept its terms.

 */


#include <vector>
#include <boost/heap/binomial_heap.hpp>

#include "morto.h"


namespace morto {


	using namespace std;



	void testOrDie(bool b, string msg)
	{
		if (!b)
			throw MortoException{ msg };
	}


	/**
	 * Generic UnionFind with path compression and union by rank on integer indicis.
	 */
	class UnionFind {
	private:
		vector<size_t> parent;
		vector<int> rank;

	public:
		/**
		Create a new family of size singleton sets.
		*/
		UnionFind(size_t size = 0)
		{
			if (size > 0)
			{
				for (size_t i = 0; i < size; ++i)
					parent.push_back(i);
				rank.resize(size, 0);
			}
		}

		/**
		Create a new singleton and return the index of its (single and canonical) node.
		Complexity O(1) (amortized)
		*/
		/*
		size_t createSet()
		{
			size_t size = parent.size();
			parent.push_back(size);
			rank.push_back(0);
			return size;
		}
		*/

		/**
		Finds and return the canonical node of the given nodeIndex and performs path compression.
		Complexity O(a(n)) (amortized)
		*/
		size_t findCanonical(size_t nodeIndex)
		{
			size_t save = nodeIndex;
			while (parent[nodeIndex] != nodeIndex)
				nodeIndex = parent[nodeIndex];
			while (parent[save] != save)
			{
				size_t tmp = save;
				save = parent[save];
				parent[tmp] = nodeIndex;
			}
			return nodeIndex;
		}

		/**
		Performs the union of two sets (given by their canonical nodes) with rank optimisation.
		Returns the index of the canonical node of the union.
		Complexity O(1)
		*/
		auto setUnion(size_t canonicalNodeIndex1, size_t canonicalNodeIndex2)
		{
			if (rank[canonicalNodeIndex1] > rank[canonicalNodeIndex2])
			{
				swap(canonicalNodeIndex1, canonicalNodeIndex2);
			}
			else if (rank[canonicalNodeIndex1] == rank[canonicalNodeIndex2])
			{
				rank[canonicalNodeIndex2]++;
			}
			parent[canonicalNodeIndex1] = canonicalNodeIndex2;
			return make_pair(canonicalNodeIndex2, canonicalNodeIndex1);
		}
	};



	//forward declaration
	struct HeapNode;

	using heap_t = boost::heap::binomial_heap<HeapNode>;

	struct HeapNode {
		double value;
		size_t nodeIndex;
	};

	bool operator<(const HeapNode & lo, const HeapNode & ro)
	{
		return lo.value < ro.value;
	}


	/**
	* Internal tree node, merges information on nodes, blocks and heaps...
	*/
	struct TreeNode {
		/**
		 * Index of the parent node
		 */
		size_t parent = 0;

		/**
		 * Weight
		 */
		double weight = 1;

		/**
		 * Input value
		 */
		double value = 0;

		/**
		 * Handle on the HeapNode associated to this node in the heap of its parent
		 */
		heap_t::handle_type handle;

		/**
		 * Heap of HeapNodes associated to every child (only valid if the node is the canonical node associated to its block)
		 */
		heap_t heap;

		/**
		 * Sum of weights of the nodes in the block (only valid if the node is the canonical node associated to its block)
		 */
		double blockTotalWeight = 1;

		/**
		* Weighted sum of the nodes in the block (only valid if the node is the canonical node associated to its block)
		*/
		double blockWeightedSum = 0;


		/**
		 * \brief Average weight of the block (only valid if the node is the canonical node associated to its block)
		 * \return Average weight of the block
		 */
		double getAverageWeight() const {
			return blockWeightedSum / blockTotalWeight;
		}



		void mergeWith(TreeNode * node)
		{
			heap.merge(node->heap);
			blockTotalWeight += node->blockTotalWeight;
			blockWeightedSum += node->blockWeightedSum;

		}
	};


	/**
	 * Internal class to represent the tree.
	 * Essentially a constructor.
	 */
	class Tree {
	private:
		vector<TreeNode> nodes;

	public:
		/**
		 * \brief Create a new tree from agnostic representation.
		 * \param parents Parent relation between nodes.
		 * Preconditions :
		 *		- parents.size() > 0
		 *		- "node i is the parent of node j" <=> parents[j] == i
		 *		- "node i is the root" <=> parents[i] == i
		 *		- nodes are in topological order : parents[i] >= i
		 * \param values Input data associated to nodes.
		 * The observed value at node i is values[i].
		 * Preconditions :
		 *		- values.size() == parents.size()
		 * \param weights Weights associated to nodes.
		 * If weights.size() == 0 , every node is assumed to have the same weight.
		 * Else, the weight of node i is weights[i].
		 * Preconditions :
		 *		- weights.size() == parents.size() || weights.empty()
		 */
		Tree(const vector<size_t> & parents, const vector<double> & values, const vector<double> & weights)
		{

			size_t nnodes = parents.size();
			nodes.resize(nnodes);


			if (!weights.empty())
			{
				for (size_t i = 0; i < nnodes; ++i)
				{
					nodes[i].weight = weights[i];
					nodes[i].blockTotalWeight = weights[i];
				}
			}

			for (size_t i = 0; i < nnodes; ++i)
			{
				nodes[i].parent = parents[i];
				nodes[i].value = values[i];
				nodes[i].blockWeightedSum = nodes[i].weight * nodes[i].value;
				if (parents[i] != i)
				{
					//nodes[parents[i]].children.push_back(i);
					nodes[i].handle = nodes[parents[i]].heap.push({ values[i], i });
				}

			}
		}

		/**
		 * \brief Get the node of index i
		 * \param i A valid node index (Precondition 0 <= i < tree.size())
		 * \return A reference to node i
		 */
		TreeNode * getNode(size_t i)
		{
			return &nodes[i];
		}

		/**
		 * \brief Number of nodes in the tree
		 * \return Number of nodes in the tree
		 */
		size_t size() const
		{
			return nodes.size();
		}
	};


	/**
	 * \brief Compute isotonic regression on the given tree.
	 * \param tree Input Tree, properly initialized.
	 * \return vector<double> containing the regression values for each node
	 */
	vector<double> IRT_BIN(Tree & tree)
	{
		size_t nnodes = tree.size();

		UnionFind uf(nnodes); // Block maintenance

		for (size_t i = 0; i < nnodes; ++i) // from leaves to root
		{


			size_t canonicali = uf.findCanonical(i); // index of the representative tree node for the block containing node i
			TreeNode * canonicalNode = tree.getNode(canonicali);


			// while we have violators among our children, fuse current block with the block of the most important violator
			while (!canonicalNode->heap.empty() && canonicalNode->getAverageWeight() < canonicalNode->heap.top().value)
			{
				size_t k = canonicalNode->heap.top().nodeIndex; // index of violator child k
				canonicalNode->heap.pop();

				size_t canonicalk = uf.findCanonical(k); // index of the representative tree node for the block containing node k


				auto resUnion = uf.setUnion(canonicali, canonicalk); // merge blocks containing i and k
				auto newCanonicalIndex = resUnion.first; // index of the new representative node (either i or k)
				auto otherIndex = resUnion.second; // index of the node that is not representative (either i or k)

				// update local variable after merge
				canonicalNode = tree.getNode(newCanonicalIndex);
				canonicali = newCanonicalIndex;

				// merge block information
				TreeNode * otherNode = tree.getNode(otherIndex);
				canonicalNode->mergeWith(otherNode);



			}

			// update parent heap to reflect the new weight of the block containing node i
			TreeNode * currentNode = tree.getNode(i);
			if (currentNode->parent != i)
			{
				TreeNode * node = tree.getNode(currentNode->parent);
				node->heap.update(currentNode->handle, { canonicalNode->getAverageWeight(), i });
			}
		}


		vector<double> res;
		for (size_t i = 0; i < nnodes; ++i)
		{
			res.push_back(tree.getNode(uf.findCanonical(i))->getAverageWeight());
		}
		return res;
	}

	bool testRelationDomain(const vector<size_t> & parents)
	{
		size_t maxV = parents.size() - 1;
		for (size_t i = 0; i < parents.size(); ++i)
		{
			if (parents[i]<0 || parents[i]>maxV) // size_t should be unsigned but well
				return false;
		}
		return true;
	}

	bool testTopologicalOrder(const vector<size_t> & parents)
	{
		for (size_t i = 0; i < parents.size(); ++i)
		{
			if (parents[i] < i)
				return false;
		}
		return true;
	}

    bool testPositiveWeights(const vector<double> & weights)
    {
        for (auto v: weights)
            if(v<=0)
                return false;
        return true;
    }
	/*bool testSingleRoot(const vector<int> & parents)
	{
		int nparents = 0;
		for (int i = 0; i<parents.size(); ++i)
		{
			if (parents[i] == i)
			{
				nparents++;
				if (nparents > 1)
					return false;
			}
		}
		return true;
	}*/


	vector<double> monotonicRegressionOnTree(const vector<size_t> & parents, const vector<double> & values, const vector<double> & weights)
	{
		testOrDie(parents.size() > 0, "Size of parents must be strictily greater than 0.");
		testOrDie(values.size() == parents.size(), "values and parents must have the same size.");
		testOrDie(weights.empty() || weights.size() == parents.size(), "weights must either be empty or have the same size as parents.");
		testOrDie(testRelationDomain(parents), "The domain of the parents relation is invalid: there exists i such that parents[i]<0 or parents[i]>=parents.size()");
		testOrDie(testTopologicalOrder(parents), "The parent relation is not given in a topological order: there exists i such that parents[i]<i");
		if(weights.size()>0)
			testOrDie(testPositiveWeights(weights),"Weights must be strictly positive.");
        //testOrDie(testSingleRoot(parents), "The parent relationcontains more than one root : there exists several i such that parents[i]==i");

		Tree tree = Tree(parents, values, weights);
		return IRT_BIN(tree);
	}


} // namespace morto
/*

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

void testIncreasing()
{
	vector<size_t> parents = { 4,4,6,6,7,7,7,7 };
	//vector<double> values = { 0,1,2,3,4,5,6,7 };
	vector<double> values = { 13,14,6,8,11,7,5,10 };
	vector<double> result = monotonicRegressionOnTree(parents, values);
	cout << result << endl;
}

int main(int argc, char ** argv)
{
	testIncreasing();
	getchar();
	return 0;
}
*/