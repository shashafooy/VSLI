#ifndef LIST_H
#define LIST_H
#include <string>
#include "wordUse.h"
#include "WordLadder.h"
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;


struct Node {
	Node(WordLadder pdata, Node* pnext, Node* plast) {
		data = pdata;
		next = pnext;
		last = plast;
	}
	WordLadder data;
	Node* next;
	Node* last;
};

class doublyLinkedList {
public:
	doublyLinkedList();
	void insertFront(WordLadder data);
	void insertBack(WordLadder data);
	void deleteFront();
	void deleteBack();
	WordLadder getFront();
	WordLadder getBack();
	string printList();
	bool isEmpty();
	void clear();
private:
	Node* head;
	Node* tail;
	stringstream ss;
};

#endif