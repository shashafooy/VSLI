#include "Game.h"


Game::Game(string inDictionary)
{
	ifstream file(inDictionary);
	string tword;
	while (file >> tword) {
		wordUse temp(tword);
		bigDictionary.push_back(temp);
	}
	file.close();
	srand(time(NULL));
	
	fout.open("output.txt");
	reset();

}

Game::~Game()
{
	fout.close();
}

void Game::listWords()
{
	for (int i = 0; i < 10; i++) {
		ss << bigDictionary[i].word << endl;
	}
	ss << endl;
	ss << "\n*******************\n\n";
	print();
}

void Game::play(string start, string end)
{
	ss << "From " << start << " to " << end << endl << endl;
	if (start.length() != end.length()) {
		//endgame
		ss << "Words are of different lengths\n\n";
		reset();
		return;
	}
	if (!subDictionaryExists) 
		createSubDictionary(start.length());
	if (!findWord(start,false) || !findWord(end,false)) {
		//endgame
		ss << "Input(s) are not valid words\n\n";
		reset();
		return;
	}

	
	print();
	targetWord = end;
	
	ladder.push_back(start);
	partialSolution.insertBack(ladder);
	bruteEnque++;
	myState.wordLadder = ladder;
	myState.priority = expectedWork(ladder, ladder[ladder.size() - 1]);
	AvlPartialSolution.insert(myState);
	AStarEnque++;

	while (true) {
		if (bruteDone&&avlDone) break;
		if (partialSolution.isEmpty() || AvlPartialSolution.isEmpty()) {
			bruteDone = false;
			break;
		}
			
		if (!bruteDone) {
			ladder = partialSolution.getFront();
			partialSolution.deleteFront();
			bruteDeque++;
		}
		if (!avlDone) {
			myState = AvlPartialSolution.removeMin();
			AStarDeque++;
		}
		if (ladder[ladder.size() - 1] == targetWord) {	bruteDone = true;	}
		if (myState.lastWord() == targetWord) {	avlDone = true;	}

		if (!bruteDone) {
			createOneAwayList(ladder[ladder.size() - 1], false);
			for (int i = 0; i < oneAwayList.size(); i++) {
				WordLadder newLadder = copyLadder();
				newLadder.push_back(oneAwayList[i]);
				partialSolution.insertBack(newLadder);
				bruteEnque++;
			}
		}
		if (!avlDone) {
			createOneAwayList(myState.lastWord(), true);
			for (int i = 0; i < oneAwayList.size(); i++) {

				WordLadderState newState = myState;
				newState.wordLadder.push_back(oneAwayList[i]);
				newState.priority = expectedWork(newState, targetWord);
				AvlPartialSolution.insert(newState);
				AStarEnque++;
			}
		}
	}
	if (bruteDone && avlDone) {
		//endgame win
		ss << "Congradulations\n";
		printLadder();
		ss << " " << ladder.size() << " BRUTE dequeued: " << bruteDeque << " enqueued: " << bruteEnque << endl;
		ss << myState.wordLadder;
		ss << " " << myState.wordLadder.size() << " AVL dequeued: " << AStarDeque << " enqueued: " << AStarEnque << endl;
		reset();
	}
	else {
		//endgame lose
		ss << "Could not find ladder for " << ladder[0];
		reset();
	}
	print();
}
//select random word of same length
void Game::play(string start)
{
	if (!subDictionaryExists) createSubDictionary(start.length());
	string end;
	do {
		end = subDictionary[rand() % subDictionary.size()].word;
	} while (end == start);

	play(start, end);
}

void Game::createSubDictionary(int wordLen)
{
	for (int i = 0; i < bigDictionary.size(); i++) {
		if (bigDictionary[i].word.length() == wordLen) {
			subDictionary.push_back(bigDictionary[i]);
		}
	}
	subDictionaryExists = true;
}
void Game::createOneAwayList(string word, bool isAvl)
{
	oneAwayList.clear();

	for (int j = 0; j < word.length(); j++) {
		for (int i = 'a'; i < 'a' + 26; i++) {
			string tempWord = word;
			tempWord[j] = i;
			if (tempWord == word || !findWord(tempWord, isAvl)) continue;
			oneAwayList.push_back(tempWord);
		}
	}
}

bool Game::findWord(string word, bool isAvl)
{
	int first, last, mid;
	first = 0;
	last = subDictionary.size()-1;
	mid = (first + last) / 2;
	while (first <= last) {
		if (subDictionary[mid].word == word) {
			if (!isAvl) {
				if (subDictionary[mid].used == true && subDictionary[mid].word != targetWord) {
					return false;
				}
				subDictionary[mid].used = true;
			}
			else {
				if (subDictionary[mid].levelUsed > myState.wordLadder.size())
					subDictionary[mid].levelUsed = myState.wordLadder.size();
				else
					return false;
			}
			return true;
		}
		if (subDictionary[mid].word.compare(word)>0) {
			//mid word is larger(later in dictionary) than target
			last = mid - 1;
		}
		else {
			//mid word is less(earlier in dictionary) than target
			first = mid + 1;
		}
		mid = (first + last) / 2;
	}
	return false;
}
//todo
WordLadder Game::copyLadder()
{
	return ladder;
}

void Game::printLadder()
{
/*	for (int i = 0; i < ladder.size(); i++) {
		ss << ladder[i] << endl;
	}
*/
	ss << "[";
	for (int i = 0; i < ladder.size(); i++) {
		if (i != 0) ss << ", ";
		ss << ladder[i];
	}
	ss << "]";
}
//call to reset variables for another game
void Game::reset()
{
	ladder.clear();
	oneAwayList.clear();
	subDictionaryExists = false;
	subDictionary.clear();
	bruteDone = false;
	avlDone = false;
	targetWord.clear();
	partialSolution.clear();
	AvlPartialSolution.makeEmpty();
	bruteDeque = 0;
	bruteEnque = 0;
	AStarDeque = 0;
	AStarEnque = 0;
	ss << "\n*******************\n\n";

	print();

}

void Game::print()
{
	if (printToCMD) cout << ss.str();
	if (printToFile) fout << ss.str();
	ss.str(string());
}

//may want to improve algorithm
int Game::expectedWork(WordLadderState curr, string endWord)
{
	int priority = curr.priority;
	string recentWord = curr.wordLadder[curr.wordLadder.size() - 1];
	for (int i = 0; i < recentWord.length(); i++) {
		//run through each letter
		//if the letter doesn't match end word letter, 
		//priority++
		if (recentWord[i] != endWord[i]) {
			priority += recentWord.length();
		}
	}
	return priority;
}

void Game::displayOneAwayWords(string word)
{ 
	createOneAwayList(word,true);

	ss << "Ladder Level: " << ladder.size() << endl;
	ss << "One away from " << word << endl;

	for (int i = 0; i < oneAwayList.size(); i++) {
		ss << oneAwayList[i] << " ";
	}
	ss << endl << endl;

	print();



}
