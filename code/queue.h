#include <iostream>
#include <string>
using namespace std;

struct proc {
	int algo = -1;
	int artime;
	int burst;
	int cTime = 0;
	int execTime = 0;
	char name[10] = {'a','n','y'};
	int sTime = 0;
	int waitingTime = 0;
	
	void print() {
		cout << "Name: " << name << endl;
		cout << "Arrival time: " << artime << endl;
		cout << "Burst: " << burst << endl;
		cout << "Start time: " << sTime << endl;
		cout << "Completion time: " << cTime << endl;
		cout << "Execution time: " << execTime << endl;
		cout << "Waiting time: " << sTime - artime << endl;
	}
};


class Queue {

	struct procNode {
		proc dataItem;
		procNode* next;
	};

	procNode* front;
	procNode* tail;
	int noOfNodes;

public:
	Queue() : front(NULL), tail(NULL), noOfNodes(0) {}

	~Queue() {			

		procNode* current = front;

		while (front != NULL) {
			current = front;
			front = front->next;
			delete current;
		}
	}

	void enqueue(const proc newDataItem) {
		procNode* temp;
		temp = new procNode;
		temp->dataItem = newDataItem;
		temp->next = NULL;

		if (isEmpty()) {
			front = temp;
			tail = temp;
		}

		else {
			tail->next = temp;
			tail = tail->next;

			if (newDataItem.algo == 1)		// SJF
				this->sort();
		}

		noOfNodes++;
	}

	proc dequeue() {

		proc val = front->dataItem;
		procNode* temp;
		temp = front;
		front = front->next;
		delete temp;

		noOfNodes--;

		return val;
	}

	bool isEmpty() const {

		if (!noOfNodes)
			return true;

		return false;
	}

	void display() {
		procNode* temp = front;

		while (temp != NULL) {
			temp->dataItem.print();
			cout << endl;
			temp = temp->next;
		}

		cout << "\n";
	}

	int Size() {
		return noOfNodes;
	}

	void sort() {
		if (isEmpty())
			return;

		procNode* current;
		procNode* current2;
		proc temp;

		for (current = front; current != NULL; current = current->next) {
			for (current2 = front; current2 != NULL; current2 = current2->next) {
				if (current->dataItem.burst < current2->dataItem.burst) {
					temp = current->dataItem;
					current->dataItem = current2->dataItem;
					current2->dataItem = temp;
				}
			}
		}
	}
	
	void incWaitingTime() {
		procNode* temp = front;

		while (temp != NULL) {
			temp->dataItem.waitingTime++;
			temp = temp->next;
		}
	}
};
