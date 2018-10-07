#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
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
	 * \brief Searches for a value
	 * \param x Value to find
	 * \return true if x is found
	 */
	bool Contains(const Comparable & x) const
	{
		return Contains(x, root);
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
	 * \brief Make the tree empty.
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
	 * \brief Try to delete the given value
	 * \param x Value to delete
	 */
	void Remove(const Comparable & x)
	{
		size--;
		Remove(x, root);
	}

	/**
	 * \brief Searches for a given value
	 * \param x Value to search for
	 * \return the height of the node
	 */
	int GetNodeHeight(const int x)
	{
		return GetHeight(GetNode(x, root));
	}

	enum PrintOrder
	{
		preOrder = 1, postOrder = 2, inOrder = 3
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
	 * \brief Insert a value into the tree
	 * \param  x Value to insert
	 * \param  t Current head
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
	 * \brief Delete value from tree.
	 * \param x The item to remove.
	 * \param t The node that roots the subtree.
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
	*\brief Find the minimum node
	*\param t Head of subtree
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

	/**
	 * \brief Balance tree using right/left rotation. Performed after every Insert/Delete
	 * \param t
	 */
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
	 * \brief Find the smallest item in a subtree.
	 * \param t Head of subtree
	 * \return node containing the smallest item.
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
	 * \brief Find the largest item in a subtree.
	 * \param t Head of subtree
	 * \return node containing the largest item.
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
	  \brief Check if an item is in a subtree.
	  \param x Item to search for.
	  \param t Head of subtree.
	 */
	bool Contains(const Comparable & x, AvlNode *t) const
	{
		if (t == nullptr)
			return false;
		if (x < t->element)
			return Contains(x, t->left);
		if (t->element < x)
			return Contains(x, t->right);

		return true;    // Match
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
		}
		t = nullptr;
	}

	/**
	 * \brief Makes a string of the tree in a "tree-like" display. Print order is set in a public method
	 * \param t Head of subtree
	 * \param spacer Space used to separate entries
	 * \return String that holds the tree
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
	 * \brief Makes a string of the tree inline. Print order is set in a public method
	 * \param t Head of subtree
	 * \return String that holds the tree
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
	 * \brief Deep Copy to replicate tree
	 * \param t Head of subtree
	 * \return Head of new tree
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
	 * \brief  Gets the height of the node.
	 * \param t Head of subtree
	 * \return  Height of node t or -1 if nullptr.
	 */
	static int GetHeight(AvlNode *t)
	{
		return t == nullptr ? -1 : t->height;
	}


	/**
	  \brief Single Rotation left child.
	  \param k2 Node to base rotation off of	
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
	  \brief Single Rotation right child.
	  \param k1 Node to base rotation off of
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
	 *\brief Double Rotation left child
	 *\param k1 Node to base rotation off of
	 */
	void DoubleWithLeftChild(AvlNode * & k3)
	{
		RotateWithRightChild(k3->left);
		RotateWithLeftChild(k3);
	}

	/**
	 *\brief Double Rotation right child
	 *\param k1 Node to base rotation off of
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
