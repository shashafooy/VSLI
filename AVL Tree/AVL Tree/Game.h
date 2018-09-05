#ifndef GAME_H
#define GAME_H
#include "avlTreeTemplate.h"
#include "doublyLinkedList.h"
#include "WordLadder.h"
#include "WordLadderState.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include "AVLTree.h"

using namespace std;

class Game {
public:
	//initialize the dictionary from the given file
	explicit Game(string inDictionary);
	~Game();
	//print first 10 words in dictionary
	void ListWords();
	void Play(string start, string end);
	void Play(const string& start);
	void DisplayOneAwayWords(string word);

	bool printToCmd = false;
	bool printToFile = false;

private:
	doublyLinkedList partialSolution;
	AvlTree<WordLadderState> avlPartialSolution;
	vector<WordUse> bigDictionary;
	WordLadder ladder;
	WordLadderState myState;
	vector<WordUse> subDictionary;
	bool bruteDone{};
	bool avlDone{};
	string targetWord;
	ofstream fout;
	WordLadder oneAwayList;
	bool subDictionaryExists{};
	stringstream ss;
	int bruteDeque{}, bruteEnque{};
	int aStarDeque{}, aStarEnque{};


	void createSubDictionary(int wordLen);
	void createOneAwayList(string word, bool isAvl);
	bool findWord(string word, bool isAvl);
	WordLadder copyLadder();
	void printLadder();
	void displayPartialList() { partialSolution.PrintList(); }
	void reset();
	void print();
	int expectedWork(WordLadderState curr, string endWord);
	

};
#endif