#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include "AVLTree.h"

class AVLProgram    // NOLINT
{
public:
	AVLProgram()
	{
		fout.open("pettit-A02107849.txt");
		tree = new AvlTree<int>();
		stop = false;
	}
	~AVLProgram()
	{
		fout.close();
		DeleteTree();
	}




	void Run()
	{
		PrintOptions();
		while (!stop)
		{

			GetCommand();
			if (!ValidateCommand())
			{
				Print("Invalid input");
				continue;
			}
			RunCommand();

		}


	}

private:
	string userInput;
	string command;
	string arg;
	bool stop;
	ofstream fout;
	AvlTree<int>* tree;

#pragma region Run Logic


	/**
	 * \brief Checks if the entered command is valid. Also checks if the argument is correct with the given command.
	 * \return True if the command is valid, else will return false
	 */
	bool ValidateCommand() const
	{
		char* p;
		auto value = strtod(arg.c_str(), &p);
		//!*p (remaining string) checks to make sure arg contains only numbers
		if (command == "Create_Tree" && !arg.empty()) return true;
		if (command == "Insertion" && !*p && !arg.empty()) return true;
		if (command == "Deletion" && !*p && !arg.empty())return true;
		if (command == "Search" && !*p && !arg.empty()) return true;
		if (command == "Traversal") return true;
		if (command == "Delete_Tree") return true;
		if (command == "Check_Balance") return true;
		if (command == "Exit") return true;

		if (command == "Pre_order" && !*p && !arg.empty()) return true;
		if (command == "Post_order" && !*p && !arg.empty()) return true;
		if (command == "In_order" && !*p && !arg.empty()) return true;


		return false;
	}

	/**
	 * \brief Gets the user command line input
	 */
	void GetCommand()
	{
		Print("\n>");
		getline(cin, userInput);
		istringstream iss(userInput);
		iss >> command >> arg;
	}

	/**
	 * \brief Executes the command
	 */
	void RunCommand()
	{
		if (command == "Create_Tree") CreateTree(arg);
		else if (command == "Insertion") Insert(stoi(arg));
		else if (command == "Deletion") Delete(stoi(arg));
		else if (command == "Search") Search(stoi(arg));
		else if (command == "Traversal") Traversal();
		else if (command == "Delete_Tree") DeleteTree();
		else if (command == "Check_Balance") CheckBalance();
		else if (command == "Exit") stop = true;
	}

	/**
	 * \brief Prints the valid input options to the user
	 */
	void PrintOptions()
	{
		string a = "Choose the option to be performed:\n";
		a.append("Create_Tree\n")
			.append("Insertion\n")
			.append("Deletion\n")
			.append("Search\n")
			.append("Traversal\n")
			.append("Delete_Tree\n")
			.append("Check_Balance\n")
			.append("Exit");
		PrintLine(a);
	}

	/**
	* \brief Prints the given string to a file and command window
	* \param str Given value to print
	*/
	void Print(const string& str)
	{
		fout << str;
		cout << str;
	}

	/**
	 * \brief Prints the given string to a file and command window followed by an end line
	 * \param str Given value to print
	 */
	void PrintLine(const string& str)
	{
		fout << str << endl;
		cout << str << endl;
	}
#pragma endregion


#pragma region Commands


	/**
	 * \brief Reads values from a .txt file, creates the tree, and prints it
	 * \param file .txt file to be read from
	 */
	void CreateTree(const string& file)
	{
		ifstream fin(file);
		string item;
		tree->MakeEmpty();
		while(!fin.eof())
		{
			getline(fin, item);
			if (item.empty()) break;
			auto value = stoi(item);
			tree->Insert(value);
		}
		Print(tree->ToString());
	}

	/**
	 * \brief Inserts the given int into the tree and prints out the tree.
	 * \param value int to be inserted into tree
	 */
	void Insert(const int value)
	{
		tree->Insert(value);
		Print(tree->ToString());
	}

	/**
	 * \brief Deletes the given int from the tree and prints out the tree.
	 * \param value int to be deleted from the tree
	 */
	void Delete(int value)
	{
		if (!tree->Contains(value))
		{
			Print("Node not found.");
			return;
		}
		tree->Remove(value);
		Print(tree->ToString());
	}

	/**
	 * \brief Prints out the height of the given value
	 * \param value value to search for
	 */
	void Search(const int value)
	{
		const auto height = tree->GetNodeHeight(value);
		height == -1 ? Print("Node not found in tree") : Print(std::to_string(height));
	}


	/**
	 * \brief Prompts user for type of traversal and prints out the tree;
	 */
	void Traversal()
	{
		Print("Select Traversal type (input as int)\n[1]Pre_order\n[2]Post_order\n[3]In_order\n");
		GetCommand();
		// ReSharper disable once CppExpressionWithoutSideEffects
		switch (stoi(command))
		{
		case 1:
			Print(tree->ToString(tree->preOrder));
			break;
		case 2:
			Print(tree->ToString(tree->postOrder));
			break;
		case 3:
			Print(tree->ToString(tree->inOrder));
			break;
		default:
			Print("Invalid Traversal Input");
			Traversal();
		}

	}

	/**
	 * \brief Deletes the tree
	 */
	void DeleteTree()
	{
		tree->MakeEmpty();
		Print("Tree Deleted");
	}

	/**
	 * \brief Prints out the current balance of the tree
	 */
	void CheckBalance()
	{
		Print(std::to_string(tree->GetBalance()));
	}
#pragma endregion



};



