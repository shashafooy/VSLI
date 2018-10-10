#pragma once
#include <sstream>
#include "Node.h"
#include <algorithm>

using namespace std;

class Tree
{
public:
	Tree() : root { nullptr }, size(0)
	{ }

	int GetSize() const { return size; }

	~Tree()
	{
		MakeEmpty();
	}


	/**
	 * \brief Check if tree is empty
	 * \return Boolean if the head is null
	 */
	bool IsEmpty() const
	{
		return root == nullptr;
	}

	/**
	 * \brief Gives a string of the tree inline
	 * \return The tree in a string
	 */
	string ToString() const
	{
		ostringstream oss;
		if (IsEmpty())
			oss << "" << endl;
		else
			oss << ToString(root);
		return oss.str();
	}

	/**
	 * \brief Make the tree empty.
	 */
	void MakeEmpty()
	{
		MakeEmpty(root);
	}

	/**
	 *	\brief Insert x into the tree;
	 */
	void Insert(const Node x)
	{
		size++;
		Insert(x, root);
	}

	/**
	 * \brief Gets all the dimensions of the head and finds the cost.
	 * \return The minimum area of the tree
	 */
	double GetHeadCost()
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		GetNodeDim(root);
		root->element.area = -1;
		//find smallest area of all possible dimensions
		for (auto dim : root->element.dims)
		{
			if (root->element.area > dim.width*dim.height || root->element.area == -1)
				root->element.area = dim.width*dim.height;
		}
		return root->element.area;
	}


private:
	struct AvlNode
	{
		Node element;
		AvlNode   *left;
		AvlNode   *right;
		int       height;

		AvlNode(const Node& ele, AvlNode *lt, AvlNode *rt, const int h = 0)
			: element { ele }, left { lt }, right { rt }, height { h } { }

	};

	AvlNode *root;
	int size;


	/**
	 * \brief Check to see if there are any redundant dimensions
	 * \param dims dimension vector to check
	 * \return true if an item was erased, false if no items found
	 */
	static bool CheckDuplicateDim(vector<Node::Dim>& dims)
	{
		for (auto val1 : dims)
		{
			for (auto val2 : dims)
			{
				//same node, skip
				if (val1 == val2) continue;
				if (val1.height == val2.height)
				{
					dims.erase(find(dims.begin(), dims.end(), (val1.width > val2.width) ? val1 : val2));
					return true;
				}
				if (val1.width == val2.width)
				{
					dims.erase(find(dims.begin(), dims.end(), (val1.height > val2.height) ? val1 : val2));
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * \brief Gets all the possible dimensions of a given Node
	 * \param t the local "head" of the tree. 
	 * \return Vector of all possible dimensions of the node.
	 */
	vector<Node::Dim> GetNodeDim(AvlNode*&t) const
	{
		vector<Node::Dim> retVal;
		Node::Dim dim {};
		if (t == nullptr)
		{
			return retVal;
		}
		//leaf
		if (t->element != 'V' && t->element != 'H')
		{
			return t->element.dims;
		}

		//H or V

		//compute node's width/height
		auto leftVal = GetNodeDim(t->left);
		auto rightVal = GetNodeDim(t->right);

		//add values of left and right dim
		for (auto &left : leftVal)
		{
			for (auto &right : rightVal)
			{
				dim.height = (t->element == 'V') ? max(left.height, right.height) : left.height + right.height;
				dim.width = (t->element == 'H') ? max(left.width, right.width) : left.width + right.width;
				retVal.push_back(dim);
			}
		}

		//check for redundant dim
		
		while(CheckDuplicateDim(retVal)){ }

		t->element.dims = retVal;

		return retVal;


	}



	/**
	 * \brief Insert a value into the tree
	 * \param  x Value to insert
	 * \param  t Current head
	 * \return True if successfully inserted the Node
	 */
	bool Insert(const Node x, AvlNode * & t) const
	{
		//TODO custom insert for NPE

		if (t == nullptr)
		{
			t = new AvlNode { x, nullptr, nullptr };
			return true;
		}
		//leaf
		if (t->element != 'H' && t->element != 'V')
			return false;

		//try inserting on the right
		if (!Insert(x, t->right))
			return Insert(x, t->left);
		return true;
	}

	/**
	 * \brief Deletes the tree
	 * \param t Head of subtree
	 */
	void MakeEmpty(AvlNode * & t)
	{
		if (t != nullptr)
		{
			MakeEmpty(t->left);
			MakeEmpty(t->right);
			delete t;
			size--;
		}
		t = nullptr;
	}

	/**
	 * \brief Makes a string of the tree postorder
	 * \param t Head of subtree
	 * \return String that holds the tree
	 */
	string ToString(AvlNode *t) const
	{
		if (t == nullptr) return "";
		ostringstream oss;

		oss << ToString(t->left);
		oss << ToString(t->right);
		oss << t->element.name;
		return oss.str();

	}


};
