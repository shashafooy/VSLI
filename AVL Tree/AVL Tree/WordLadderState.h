#ifndef WL_STATE
#define WL_STATE
#include<string>
#include "WordLadder.h"


class WordLadderState{
public:
	WordLadder wordLadder;
	int priority;

	string lastWord() {
		return wordLadder[wordLadder.size() - 1];
	}
	WordLadderState(WordLadder tWordLadder, int tPriority = 0)
		: wordLadder{ tWordLadder }, priority{ tPriority } {	}
	WordLadderState() { priority = 0; }

	friend bool operator==(const WordLadderState& left, const WordLadderState& right) {
		return left.priority == right.priority;
	}
	friend bool operator<(const WordLadderState& left, const WordLadderState& right) {
		return left.priority < right.priority;
	}
	friend bool operator>(const WordLadderState& left, const WordLadderState& right) {
		return left.priority > right.priority;
	}
};


#endif