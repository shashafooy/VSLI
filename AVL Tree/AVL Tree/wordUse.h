#ifndef WORDUSE_H
#define WORDUSE_H
#include<string>

using namespace std;

class wordUse {
public:
	wordUse(string tword, bool tused = false) {
		word = tword;
		used = tused;
		levelUsed = 1000000; //larger than dictionary
	}
	string word;
	bool used;
	int levelUsed;
};

#endif
