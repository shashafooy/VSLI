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
	AVLProgram()
	{
		fout.open("pettit-A02107849.txt");
		tree = new AvlTree<int>();
	}
	~AVLProgram()
	{
		fout.close();
		DeleteTree();
	}
public:



	void Run()
	{
		PrintOptions();
		while (true)
		{

			GetCommand();
			if (!ValidateCommand())
			{
				Print("Invalid input\n");
				continue;
			}
			RunCommand(command, arg);

		}


	}

private:
	string userInput;
	string command;
	string arg;
	ofstream fout;
	AvlTree<int>* tree;


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
		Print(">");
		getline(cin, userInput);
		istringstream iss(userInput);
		iss >> command >> arg;
	}

	/**
	 * \brief Executes the command
	 * \param cmd Given Command
	 * \param arg Given Argument
	 */
	void RunCommand(const string& cmd, const string& arg)
	{
		if (cmd == "Create_Tree") CreateTree(arg);
		else if (cmd == "Insertion") Insert(stoi(arg));
		else if (cmd == "Deletion") Delete(stoi(arg));
		else if (cmd == "Search") Search(stoi(arg));
		else if (cmd == "Traversal") Traversal();
		else if (cmd == "Delete_Tree") DeleteTree();
		else if (cmd == "Check_Balance") CheckBalance();
	}

	/**
	 * \brief Prints the valid input options to the user
	 */
	void PrintOptions()
	{
		string a = "Choose the option to be performed:\nCreate_Tree\n";
		a.append("Create_Tree\n")
			.append("Insertion\n")
			.append("Deletion\n")
			.append("Search\n")
			.append("Traversal\n")
			.append("Delete_Tree\n")
			.append("Check_Balance");
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

	void CreateTree(const string& file);

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
		tree->Remove(value);
		Print(tree->ToString());
	}

	/**
	 * \brief Prints out the height of the given value
	 * \param value value to search for
	 */
	void Search(const int value) const
	{
		tree->Contains(value);
		//todo: Print the node's height
	}


	/**
	 * \brief Prompts user for type of traversal and prints out the tree;
	 */
	void Traversal()
	{
		Print("Select Traversal type (input as int)\n1.Pre_order\n2.Post_order\n3.In_order\n");
		GetCommand();
		// ReSharper disable once CppExpressionWithoutSideEffects
		switch(stoi(command))
		{
		case 1:
			//todo: PreOrder
			break;
		case 2:
			//todo: PostOrder
			break;
		case 3:
			//todo: InOrder
			break;
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
		//todo: give AVLTree a balance variable/function
		Print(tree->GetBalance());
	}
	

};



