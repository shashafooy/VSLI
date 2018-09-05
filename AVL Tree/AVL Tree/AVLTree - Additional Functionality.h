#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <iostream> 
#include <cassert>
#include <sstream>
#include <utility>
using namespace std;


template <class Comparable>
class AvlTree
{
#pragma region public

public:
	AvlTree() : root { nullptr }
	{
		size = 0;
		printOrder = preOrder;
	} 

	int GetSize() const { return size; }

	AvlTree(const AvlTree & rhs) : root { nullptr }
	{
		size = rhs.size;
		root = Clone(rhs.root);
	}


	AvlTree(AvlTree && rhs) noexcept : root { rhs.root }, size(0)
	{
		rhs.root = nullptr;
	}

	~AvlTree()
	{
		MakeEmpty();
	}

	/* Deep copy. */
	AvlTree & operator=(const AvlTree & rhs)
	{
		AvlTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}


	int GetBalance()
	{
		return IsEmpty() ? -1 : abs(root->left->height - root->right->height);
	}

	/**
	 * \brief Find the smallest item in the tree. Throw UnderflowException if empty.
	 * \return The minimum object in the tree
	 */
	const Comparable & FindMin() const
	{
		assert(!IsEmpty());
		return FindMin(root)->element;
	}

	/* Find the largest item in the tree.	 */
	const Comparable & FindMax() const
	{
		assert(!IsEmpty());
		return FindMax(root)->element;
	}

	/* Returns true if x is found in the tree. */
	bool Contains(const Comparable & x) const
	{
		return Contains(x, root);
	}

	/* Test if the tree is logically empty.	 */
	bool IsEmpty() const
	{
		return root == nullptr;
	}

	/**
	 * \brief Gives a string of the tree inline
	 * \param traversalType 1: PreOrder	 2: PostOrder  3: InOrder
	 * \return The tree in a string
	 */
	string ToString(int traversalType = 1)
	{
		printOrder = static_cast<PrintOrder>(traversalType);
		ostringstream oss;
		if (IsEmpty())
			oss << "Empty tree" << endl;
		else
			oss << ToString(root);
		return oss.str();
	}

	/**
	 * \brief Make the tree logically empty.
	 */
	void MakeEmpty()
	{
		MakeEmpty(root);
	}

	/**
	 *	\brief Insert x into the tree;
	 */
	void Insert(const Comparable & x)
	{
		if (Contains(x)) return;
		size++;
		Insert(x, root);
	}

	/**
	 * Insert x into the tree;
	 * && is termed an rvalue reference.
	 * a good explanation is at http://thbecker.net/articles/rvalue_references/section_01.html
	 */
	void Insert(Comparable && x)
	{
		if (Contains(x)) return;

		size++;
		Insert(std::move(x), root);
	}

	/* Remove x from the tree. Nothing is done if x is not found.	 */
	void Remove(const Comparable & x)
	{
		size--;
		Remove(x, root);
	}
	/* remove smallest element from the tree.  Return the value found there*/
	Comparable RemoveMin()
	{
		size--;
		AvlNode * min = RemoveMin(root);
		assert(min != NULL);
		return min->element;
	}

	int GetNodeHeight(const int x)
	{
		return GetHeight(GetNode(x, root));
	}

	enum PrintOrder
	{
		preOrder=1, postOrder=2, inOrder=3
	};

#pragma endregion

#pragma region Private


private:


	struct AvlNode
	{
		Comparable element;
		AvlNode   *left;
		AvlNode   *right;
		int       height;

		AvlNode(Comparable ele, AvlNode *lt, AvlNode *rt, const int h = 0)
			: element { std::move(ele) }, left { lt }, right { rt }, height { h } { }

		AvlNode(Comparable && ele, AvlNode *lt, AvlNode *rt, const int h = 0)
			: element { std::move(ele) }, left { lt }, right { rt }, height { h } { }
	};

	AvlNode *root;
	int size;
	PrintOrder printOrder;



	/**
	 * Internal method to insert into a subtree.
	 * x is the item to insert.
	 * t is the node that roots the subtree.
	 * Set the new root of the subtree.
	 */
	void Insert(const Comparable & x, AvlNode * & t)
	{
		if (t == nullptr)
			t = new AvlNode { x, nullptr, nullptr };
		else if (x < t->element)
			Insert(x, t->left);
		else
			Insert(x, t->right);

		Balance(t);
	}

	/**
	 * Internal method to insert into a subtree.
	 * x is the item to insert.
	 * t is the node that roots the subtree.
	 * Set the new root of the subtree.
	 */
	void Insert(Comparable && x, AvlNode * & t)
	{
		if (t == nullptr)
			t = new AvlNode { std::move(x), nullptr, nullptr };
		else if (x < t->element)
			Insert(std::move(x), t->left);
		else
			Insert(std::move(x), t->right);

		Balance(t);
	}

	/**
	 * Internal method to remove from a subtree.
	 * x is the item to remove.
	 * t is the node that roots the subtree.
	 * Set the new root of the subtree.
	 */
	void Remove(const Comparable & x, AvlNode * & t)
	{
		if (t == nullptr)
			return;   // Item not found; do nothing

		if (x < t->element)
			Remove(x, t->left);
		else if (t->element < x)
			Remove(x, t->right);
		else if (t->left != nullptr && t->right != nullptr) // Two children
		{
			t->element = FindMin(t->right)->element;
			Remove(t->element, t->right);
		}
		else
		{
			AvlNode *oldNode = t;
			t = (t->left != nullptr) ? t->left : t->right;
			delete oldNode;
		}

		Balance(t);
	}
	/**
	*Internal method to find the minimum node
	*store the node in a temp node
	*delete the original node
	*return temp node
	*/
	AvlNode*  RemoveMin(AvlNode * & t)
	{
		if (t == nullptr) return nullptr;

		if (t->left == nullptr)
		{
			AvlNode *retNode = Clone(t);
			Remove(t->element, t);
			return retNode;
		}
		return RemoveMin(t->left);
	}

	static const int ALLOWED_IMBALANCE = 1;

	// Assume t is balanced or within one of being balanced
	void Balance(AvlNode * & t)
	{
		if (t == nullptr)
			return;

		if (GetHeight(t->left) - GetHeight(t->right) > ALLOWED_IMBALANCE)
			if (GetHeight(t->left->left) >= GetHeight(t->left->right))
				RotateWithLeftChild(t);
			else
				DoubleWithLeftChild(t);
		else
			if (GetHeight(t->right) - GetHeight(t->left) > ALLOWED_IMBALANCE)
			{
				if (GetHeight(t->right->right) >= GetHeight(t->right->left))
				{
					RotateWithRightChild(t);
				}
				else
				{
					DoubleWithRightChild(t);
				}
			}

		t->height = max(GetHeight(t->left), GetHeight(t->right)) + 1;

	}

	/**
	 * Internal method to find the smallest item in a subtree t.
	 * Return node containing the smallest item.
	 */
	AvlNode * FindMin(AvlNode *t) const
	{
		if (t == nullptr)
			return nullptr;
		if (t->left == nullptr)
			return t;
		return FindMin(t->left);
	}

	/**
	   Internal method to find the largest item in a subtree t.
	   Return node containing the largest item.
	 */
	static AvlNode * FindMax(AvlNode *t)
	{
		if (t != nullptr)
			while (t->right != nullptr)
				t = t->right;
		return t;
	}

	/**
	 * \brief Gets the desired AVL Node
	 * \param x Value to search for
	 * \param t Current head of the tree
	 * \return 
	 */
	AvlNode * GetNode(const Comparable & x, AvlNode *t) const
	{
		if (t == nullptr) return t;
		if (x < t->element)
			return GetNode(x, t->left);
		if (x > t->element)
			return GetNode(x, t->right);
		return t;
	}


	/**
	  Internal method to test if an item is in a subtree.
	  x is item to search for.
	  t is the node that roots the tree.
	 */
	bool Contains(const Comparable & x, AvlNode *t) const
	{
		if (t == nullptr)
			return false;
		else if (x < t->element)
			return Contains(x, t->left);
		else if (t->element < x)
			return Contains(x, t->right);
		else
			return true;    // Match
	}

	/**
	  Internal method to make subtree empty.
	 */
	void MakeEmpty(AvlNode * & t)
	{
		if (t != nullptr)
		{
			MakeEmpty(t->left);
			MakeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}

	/**
	  Internal method to create a string for a subtree rooted at t in sorted order.
	 */
	string ToString2(AvlNode *t, const string& spacer = "") const
	{
		if (t == nullptr) return "";
		ostringstream oss;
		switch (printOrder)
		{
		case preOrder:
			oss << spacer << t->element << endl;
			oss << ToString2(t->left, spacer + "\t");
			oss << ToString2(t->right, spacer + "\t");
			break;
		case inOrder:
			oss << ToString2(t->left, spacer + "\t");
			oss << spacer << t->element << endl;
			oss << ToString2(t->right, spacer + "\t");
			break;
		case postOrder:
			oss << ToString2(t->left, spacer + "\t");
			oss << ToString2(t->right, spacer + "\t");
			oss << spacer << t->element << endl;
			break;
		}
		return oss.str();
	}

	/**
	*Internal method to create a string for a subtree as a list of elements
	*/
	string ToString(AvlNode *t) const
	{
		if (t == nullptr) return "";
		ostringstream oss;

		switch (printOrder)
		{
		case preOrder:
			oss << " " << t->element << " ";
			oss << ToString(t->left);
			oss << ToString(t->right);
			break;
		case inOrder:
			oss << ToString(t->left);
			oss << " " << t->element << " ";
			oss << ToString(t->right);
			break;
		case postOrder:
			oss << ToString(t->left);
			oss << ToString(t->right);
			oss << " " << t->element << " ";
			break;
		}
		return oss.str();

	}

	/**
	 * Internal method to clone subtree.
	 */
	AvlNode * Clone(AvlNode *t) const
	{
		if (t == nullptr)
			return nullptr;
		else
			return new AvlNode { t->element, Clone(t->left), Clone(t->right), t->height };
	}
#pragma region AVL Manipulations
	/**
	 * Return the height of node t or -1 if nullptr.
	 */
	static int GetHeight(AvlNode *t)
	{
		return t == nullptr ? -1 : t->height;
	}

	static int Max(int lhs, int rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}

	/**
	  Rotate binary tree node with left child.
	  For AVL trees, this is a single rotation for case 1.
	  Update heights, then set new root.
	 */
	void RotateWithLeftChild(AvlNode * & k2)
	{
		AvlNode *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height = max(GetHeight(k2->left), GetHeight(k2->right)) + 1;
		k1->height = max(GetHeight(k1->left), k2->height) + 1;
		k2 = k1;
	}

	/**
	  Rotate binary tree node with right child.
	  For AVL trees, this is a single rotation for case 4.
	  Update heights, then set new root.
	 */
	void RotateWithRightChild(AvlNode * & k1)
	{
		AvlNode *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1->height = max(GetHeight(k1->left), GetHeight(k1->right)) + 1;
		k2->height = max(GetHeight(k2->right), k1->height) + 1;
		k1 = k2;
	}

	/**
	  Double rotate binary tree node: first left child.
	  with its right child; then node k3 with new left child.
	  For AVL trees, this is a double rotation for case 2.
	  Update heights, then set new root.
	 */
	void DoubleWithLeftChild(AvlNode * & k3)
	{
		RotateWithRightChild(k3->left);
		RotateWithLeftChild(k3);
	}

	/**
	  Double rotate binary tree node: first right child.
	  with its left child; then node k1 with new right child.
	  For AVL trees, this is a double rotation for case 3.
	  Update heights, then set new root.
	 */
	void DoubleWithRightChild(AvlNode * & k1)
	{
		RotateWithLeftChild(k1->right);
		RotateWithRightChild(k1);
	}
#pragma endregion

#pragma endregion

};

#endif
