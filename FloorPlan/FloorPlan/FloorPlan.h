#pragma once
#include <string>
#include <utility>
#include <fstream>
#include <vector>
#include "Tree.h"
#include "Node.h"
#include <algorithm>
#include <random>

using namespace std;


class FloorPlan
{
public:
	/**
	 * \brief Opens and parses the contents of inFile
	 * \param inFile .txt file to parse
	 */
	explicit FloorPlan(const string& inFile)
		: nMoves(10), iSeed(3), ratio(0.85), t0(-1), lambdaTf(0.005), probability(0.99), epsilon(0.00001)
	{
		in.open(inFile);
		if (!in.is_open())
		{
			printf("Unable to open file: %s\n", inFile.c_str());
			exit(EXIT_FAILURE);
		}
		ParseHeader();
		numModules = static_cast<int>(items.size());
		N = nMoves * numModules;
		srand(iSeed);
	}

	/**
	 * \brief Finds a more optimal solution based on the simulated annealing algorithm.
	 * The program will stop when either the temperature is below a set epsilon
	 *  or the ratio of rejections/moves is greater than the set ratio.
	 * \param initialSolution Initial NPE to optimize.
	 */
	void Run(const string& initialSolution)
	{
		NPELength = static_cast<int>(initialSolution.length());
		bestSolution = initialSolution;
		currSolution = initialSolution;
		if (t0 == -1) InitTemp();


		// ReSharper disable once CppInconsistentNaming
		int MT, uphill, reject;
		auto temp = -t0;
		auto currentCost = Cost(currSolution);
		auto bestCost = currentCost;

		printf("Initial Solution\n%s\nCost: %f\n\n", initialSolution.c_str(), currentCost);


		do
		{
			MT = uphill = reject = 0;
			do
			{
				const auto newE = GetMove();
				MT++;
				const auto delCost = Cost(newE) - currentCost;
				//continue if a decrease in cost or if random 0-1 < e^-cost/temp
				if (delCost < 0 || (rand() % 10000) / 10000.0 < exp(delCost / -temp))
				{
					if (delCost > 0) uphill++;
					currSolution = newE;
					currentCost = Cost(currSolution);
					if (currentCost < bestCost)
					{
						bestSolution = currSolution;
						bestCost = Cost(bestSolution);
					}
				}
				else
				{
					reject++;
				}

			} while (uphill < N && MT < 2 * N);
			temp *= lambdaTf;
		} while (reject / static_cast<double>(MT) < ratio && temp > epsilon);

		printf("Final Solution\n%s\nCost: %f\n\n", bestSolution.c_str(), bestCost);

	}

	// ReSharper disable once CppInconsistentNaming
	/**
	 * \brief Generates a tree for the given NPE and finds the cost
	 * \param NPE NPE used to create the tree
	 * \return cost of the tree
	 */
	double Cost(const string& NPE)
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

#pragma region variables
	vector<Node*> items {};
	ifstream in {};
	Tree tree {};
	/**
	 * \brief Number of attempt moves per temperature iteration. Use 10 as the default value.
	 * Larger values of nmoves in general produce better results. The total execution time of
	 * the program grows linearly with respect to nmoves.
	 */
	int nMoves;
	/**
	 * \brief Seed for random number generator
	 */
	int iSeed;
	int numModules;
	// ReSharper disable once CppInconsistentNaming
	/**
	 * \brief N=nMoves * numModules
	 */
	int N;
	/**
	 * \brief Ratio for decreasing temperatures. Use 0.85 as the default value.
	 * Larger values of ratios in general produce better results and have much longer execution time.
	 */
	double ratio;
	/**
	 * \brief Initial temperature. If the input t0 is negative (e.g. -1), the program will
	 * compute an initial temperature (in this case you will need to add
	 * a negative sign to make the initial temperature positive);
	 * otherwise the program will use the input positive t0 as the initial temperature.
	 * Default value is -1.
	 */
	double t0;
	/**
	 * \brief The program will change ratio to 0.1 when temperature falls below lambdatf * t0.
	 * After ratio is set to 0.1, the program in general will terminate very quickly.
	 */
	double lambdaTf;
	/**
	 * \brief Probability of accepting an uphill move
	 */
	double probability;
	double epsilon;
	// ReSharper disable once CppInconsistentNaming
	int NPELength {};

	string currSolution;
	string bestSolution;
#pragma endregion


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
			items.push_back(new Node(s1.c_str()[0], stof(s2), stof(s3)));
		}
	}

	/**
	 * \brief Initializes the temperature by finding the average uphill cost for the starting solution
	 */
	void InitTemp()
	{
		// ReSharper disable once CppInconsistentNaming
		auto MT { 0 }, uphill { 0 };
		double uphillCost = 0;
		const auto cost = Cost(currSolution);
		do
		{
			const auto newE = GetMove();
			MT++;
			const auto delCost = Cost(newE) - cost;
			if (delCost > 0)
			{
				uphill++;
				uphillCost += delCost;
			}

		} while (uphill < N && MT < 2 * N);
		uphillCost /= uphill;
		t0 = uphillCost / log(probability);
	}


	// ReSharper disable once CppNotAllPathsReturnValue
	/**
	 * \brief Calls moves 1-3 randomly
	 * \return string containing the result of the move
	 */
	string GetMove()
	{
		switch (rand() % 3)
		{
		case 0:
			return Move1();
		case 1:
			return Move2();
		case 2:
			return Move3();
		default: break;
		}
		return "";
	}

	/**
	 * \brief Swap adjacent operands
	 * \return New solution
	 */
	string Move1()
	{
		int index = rand() % NPELength;
		//find operand
		while (IsOperator(index))
		{
			index++;
			if (index == NPELength) index = rand() % NPELength;
		}
		const auto firstChar = index;
		//find adjacent operand
		do
		{
			index++;
			//reached end of NPE, try again
			if (index == NPELength) return Move1();
		} while (IsOperator(index));
		auto retVal = currSolution;
		swap(retVal[index], retVal[firstChar]);


		return retVal;
	}

	/**
	 * \brief complement operator chain
	 * \return New solution
	 */
	string Move2()
	{
		auto index1 = rand() % NPELength;
		//find operator
		while (IsOperand(index1))
		{
			index1++;
			if (index1 == NPELength) index1 = rand() % NPELength;
		}

		auto index2 = index1;
		//get operator chain
		do
		{
			index2++;
			//reached end of NPE, decrement and break
			if (index2 == NPELength)
			{
				break;
			}
		} while (IsOperator(index2));
		//overstepped by 1
		index2--;
		auto retVal = currSolution;
		for (; index1 <= index2; index1++)
		{
			retVal[index1] = (retVal.at(index1) == 'H') ? 'V' : 'H';
		}

		return retVal;
	}

	/**
	 * \brief Swap adjacent operator and operand
	 * \return New solution
	 */
	string Move3()
	{
		auto index = rand() % (NPELength - 1);
		auto attempts = NPELength;

		while (attempts > 0)
		{
			//find adjacent operand and operator
			while (IsOperand(index) && IsOperand(index + 1) || IsOperator(index) && IsOperator(index + 1))
			{
				index++;
				index %= NPELength - 1;
			}

			//check balloting property
			if (index > 0)
			{
				//invalid, try another move
				//()?: if item is operand compare neighboring items, 
				//if operator, check item 2 spaces ahead
				if (!(currSolution[IsOperand(index)?index - 1:index] != currSolution[IsOperand(index)?index + 1:index+2] && 2 * GetNumOperators(index + 1) <= index))
				{
					//try move 3 at next index
					if (--attempts > 0)
					{
						index++;
						index %= NPELength - 1;
						continue;
					}
					return GetMove();
				}
			}
			else
			{
				//invalid, try another move
				if (!(2 * GetNumOperators(index + 1) <= index))
				{
					//try move 3 at next index
					if (--attempts > 0)
					{
						index++;
						//check end of NPE
						index %= NPELength - 1;
						continue;
					}
					if (--attempts > 0) continue;
					return GetMove();
				}
			}
			break;
		}
		auto retVal = currSolution;
		swap(retVal[index], retVal[index + 1]);
		return retVal;
	}

	/**
	 * \brief Get the number of operators in the current solution up to a specified index
	 * \param index Index to stop looking (look at this index)
	 * \return number of operators
	 */
	int GetNumOperators(const int index)
	{
		auto retVal = 0;
		for (auto i = 0; i <= index; i++)
		{
			if (IsOperator(i)) retVal++;
		}
		return retVal;
	}

	/**
	 * \brief Determines if the current solution is an Operand
	 * \param index Char to look at in the current solution string
	 * \return true if the character is not V or H
	 */
	bool IsOperand(const int index)
	{
		if (index >= NPELength || index < 0) return false;
		return currSolution.at(index) != 'V' && currSolution.at(index) != 'H';
	}

	/**
	 * \brief Determines if the current solution is an Operator
	 * \param index Char to look at in the current string
	 * \return True if the character is a V or H
	 */
	bool IsOperator(const int index)
	{
		if (index >= NPELength || index < 0) return false;
		return currSolution.at(index) == 'V' || currSolution.at(index) == 'H';
	}

	/**
	 * \brief Creates a tree from a given NPE
	 * \param NPE string holding the NPE (12V3H...)
	 */
	void GenerateTree(string NPE)
	{
		tree.MakeEmpty();
		Node* temp;
		for (auto i = static_cast<int>(NPE.length()) - 1; i >= 0; i--)
		{
			//delete temp;
			const auto value = NPE.at(i);
			if (value == 'H' || value == 'V')
				temp = new Node(value);
			else
			{
				//lookup the block
				auto it = find_if(items.begin(), items.end(), [&](Node* node)
				{
					return node->name == value;
				});
				if (it != items.end())
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



