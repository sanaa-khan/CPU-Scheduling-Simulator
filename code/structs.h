#include <iostream>
using namespace std;

struct proc {
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


