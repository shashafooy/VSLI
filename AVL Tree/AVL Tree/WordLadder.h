#ifndef W_LADDER
#define W_LADDER
#include <vector>
#include <sstream>
#include <string>

using namespace std;

class WordLadder : public vector<string> {
public:
	friend ostream& operator<<(ostream& ss, WordLadder& gs) {
		ss << gs.toString();
		return ss;
	}
	string toString() {
		ostringstream oss;
		oss << "[";
		for (int i = 0; i < size(); i++) {
			if (i != 0) oss << ", ";
			oss << at(i);
		}
		oss<<"]";
		return oss.str();
	}
private:

};


#endif
