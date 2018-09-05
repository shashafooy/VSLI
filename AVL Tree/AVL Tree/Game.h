#ifndef GAME_H
#define GAME_H
#include "avlTreeTemplate.h"
#include "doublyLinkedList.h"
#include "WordLadder.h"
#include "WordLadderState.h"
#include <Vector>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <sstream>

using namespace std;

class Game {
public:
	//initialize the dictionary from the given file
	Game(string inDictionary);
	~Game();
	//print first 10 words in dictionary
	void listWords();
	void play(string start, string end);
	void play(string start);
	void displayOneAwayWords(string word);

	bool printToCMD = false;
	bool printToFile = false;

private:
	doublyLinkedList partialSolution;
	AvlTree<WordLadderState> AvlPartialSolution;
	vector<wordUse> bigDictionary;
	WordLadder ladder;
	WordLadderState myState;
	vector<wordUse> subDictionary;
	bool bruteDone;
	bool avlDone;
	string targetWord;
	ofstream fout;
	WordLadder oneAwayList;
	bool subDictionaryExists;
	stringstream ss;
	int bruteDeque, bruteEnque;
	int AStarDeque, AStarEnque;


	void createSubDictionary(int wordLen);
	void createOneAwayList(string word, bool isAvl);
	bool findWord(string word, bool isAvl);
	WordLadder copyLadder();
	void printLadder();
	void displayPartialList() { partialSolution.printList(); }
	void reset();
	void print();
	int expectedWork(WordLadderState ladder, string endWord);
	

};
#endif