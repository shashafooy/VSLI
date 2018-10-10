#pragma once
#include <string>
#include <utility>
//#include "AVLTree.h"
#include <fstream>
#include <vector>
#include "Tree.h"
#include "Node.h"
#include <algorithm>
#include <set>

using namespace std;

class FloorPlan
{
public:
	/**
	 * \brief Opens and parses the contents of inFile
	 * \param inFile .txt file to parse
	 */
	explicit FloorPlan(const string& inFile)
	{
		in.open(inFile);
		if (!in.is_open())
		{
			printf("Unable to open file: %s\n", inFile.c_str());
			exit(EXIT_FAILURE);
		}
		ParseHeader();
	}

	// ReSharper disable once CppInconsistentNaming
	/**
	 * \brief Generates a tree for the given NPE and finds the cost
	 * \param NPE NPE used to create the tree
	 * \return cost of the tree
	 */
	double Cost(const string NPE)
	{
		GenerateTree(NPE);

		return tree.GetHeadCost();
	}

	// ReSharper disable once CppInconsistentNaming
	/**
	 * \brief Post order traversal of the tree
	 * \return string holding the NPE
	 */
	string GetNPE() const
	{
		return tree.ToString();
	}


private:
	
	vector<Node*> items {};
	ifstream in {};
	Tree tree {};


	/**
	 * \brief Parses the header for the nodes and their sizes
	 */
	void ParseHeader()
	{
		string line;
		string s1, s2, s3;

		while (getline(in, line))
		{

			istringstream(line) >> s1 >> s2 >> s3;
			items.push_back(new Node(s1.c_str()[0],stof(s2),stof(s3)));
		}
	}

	/**
	 * \brief Creates a tree from a given NPE
	 * \param NPE string holding the NPE (12V3H...)
	 */
	void GenerateTree(string NPE)
	{
		tree.MakeEmpty();
		Node* temp = nullptr;
		for(int i = NPE.length()-1; i>=0; i--)
		{
			//delete temp;
			const char value = NPE.at(i);
			if (value == 'H' || value == 'V')
				temp = new Node(value);
			else
			{
				//lookup the block
				auto it = find_if(items.begin(), items.end(), [&](Node* node)
				{
					return node->name == value;
				});
				if(it!=items.end())
					temp = *it;
				else
				{
					printf("item %c not found in database", value);
					exit(EXIT_FAILURE);
				}
			}
			tree.Insert(*temp);
			if (temp->name == 'V' || temp->name == 'H') delete temp;
		}
	}

};



