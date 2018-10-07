#pragma once
#include <string>
#include <utility>
#include "AVLTree.h"
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
	FloorPlan(const string& inFile)
	{
		in.open(inFile);
		if (!in.is_open())
		{
			printf("Unable to open file: %s\n", inFile.c_str());
			exit(EXIT_FAILURE);
		}
		ParseHeader();
	}

	double Cost(string NPE)
	{
		GenerateTree(NPE);


		return tree.GetHeadCost();
	}

	string GetNPE()
	{

	}


private:
	
	vector<Node*> items;
	ifstream in;
	Tree tree {};


	void ParseHeader()
	{
		string line;
		Node *buffer;
		string s1, s2, s3;

		while (getline(in, line))
		{

			istringstream(line) >> s1 >> s2 >> s3;
			items.push_back(new Node(s1.c_str()[0],stof(s2),stof(s3)));
		}
	}

	void GenerateTree(string NPE)
	{
		tree.MakeEmpty();
		auto temp = new Node();
		for(int i = NPE.length()-1; i>=0; i--)
		{
			delete temp;
			const char value = NPE.at(i);
			if (value == 'H' || value == 'V')
				temp = new Node(value);
			else
			{
				auto it = find_if(items.begin(), items.end(), [&](Node* node)
				{
					return node->name == value;
				});
				temp = *it;
			}
			tree.Insert(*temp);
		}
	}

};



