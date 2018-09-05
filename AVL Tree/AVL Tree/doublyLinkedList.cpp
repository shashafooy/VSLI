#include "doublyLinkedList.h"


doublyLinkedList::doublyLinkedList()
{
	head = NULL;
	tail = NULL;
}

void doublyLinkedList::insertFront(WordLadder data)
{
	if (!head) {
		head = new Node(data, NULL, NULL);
		tail = head;
	}
	else {
		Node* newNode = new Node(data,head,NULL);
		if (!this->tail) this->tail = newNode;
		if (!this->head) this->head = newNode;
		head = newNode;
		if (!head->next) tail = head;

	}
}

void doublyLinkedList::insertBack(WordLadder data)
{
	if (!head) {
		insertFront(data);
	}
	else {
		Node* newNode = new Node(data,NULL,tail);
		tail->next = newNode;
		tail = newNode;
	}
}

void doublyLinkedList::deleteFront()
{
	if (!head) return;
	Node* temp = head;
	head = head->next;
	delete temp;
	if (!head) tail = NULL;
}


void doublyLinkedList::deleteBack()
{
	if (!tail) return;
	Node* temp = tail;
	tail = tail->last;
	delete[] temp;
	if (!tail) head = NULL;
}


WordLadder doublyLinkedList::GetFront() const
{
	return head->data;
}


WordLadder doublyLinkedList::GetBack() const
{
	return tail->data;
}


string doublyLinkedList::PrintList()
{
	Node* curHead = head;
	if (!head) ss << "empty List" << endl;
	while (curHead!=NULL) {
		ss << "[";
		for (int i = 0; i < curHead->data.size(); i++) {
			if (i != 0) ss << ", ";
			ss << curHead->data[i];
		}
		ss << "]";
		curHead = curHead->next;
	}
	ss << endl << endl;

	return ss.str();
}

bool doublyLinkedList::isEmpty()
{
	return head == nullptr;
}

void doublyLinkedList::clear()
{
	while (head != nullptr) deleteFront();
}


