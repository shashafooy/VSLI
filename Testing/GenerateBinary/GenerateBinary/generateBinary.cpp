#include <iostream>
#include <cmath>
#include <random>
#include <time.h>
#include <bitset>

using namespace std;


int main()
{
	srand(time(NULL));
	int numBits, vectors;
	int64_t number;
	cout << "Enter number of bits: ";
	cin >> numBits;
	cout << "Enter number of vectors: ";
	cin >> vectors;
	string bits;
	int64_t mod = pow(2, numBits);

	for (int i = 0; i < vectors; i++)
	{
		number = rand()<<0xf;
		number = number<<0xf | rand() << 0xf | rand();//rand() is 15 bit
		bits = bitset<64>(number).to_string();
		bits.erase(0, 64 - numBits);
		cout << bits << endl;
	}
	system("pause");
}