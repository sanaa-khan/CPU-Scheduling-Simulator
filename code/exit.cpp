 #include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <bits/stdc++.h>
#include <sstream>
#include <time.h>
#include <vector>
#include "queue.h"
//#include "structs.h"
using namespace std;


int main(int argc, char* argv[]) {

	cout << endl;

	int readfd;
	istringstream(argv[1]) >> readfd;
	
	Queue procQueue;
	vector<proc> procList;
	
	// reading completed processes from running
	
	proc tempProc;
	
	while (tempProc.artime != -1) {
		read(readfd, &tempProc, sizeof(tempProc));
		
		if (tempProc.artime != -1)
			procQueue.enqueue(tempProc);
	}

//	procQueue.display();	

	float throughput = 0, totWait = 0, totTurnAround= 0;
	
	// writing statistics to output screen //
	
	cout << "Proc Name\t | \tArrival Time\t | \tBurst Time\t | \tTurnaround Time\t | \tWaiting Time\t |" << endl;
	
	while (!procQueue.isEmpty()) {
		tempProc = procQueue.dequeue();
		procList.push_back(tempProc);
		
		throughput++;
		totWait += tempProc.waitingTime;
		totTurnAround += tempProc.cTime - tempProc.sTime;
		
		cout << tempProc.name << "\t\t | \t" << tempProc.artime << "\t\t | \t" << tempProc.burst << "\t\t | \t" << tempProc.cTime - tempProc.sTime << "\t\t | \t" << tempProc.waitingTime << "\t\t |" << endl;
	}
	
	cout << endl;
	cout << "Throughput\t | \tAverage Waiting Time\t | \tAverage Turnaround\t |" << endl;
	cout << throughput << "\t\t | \t" << totWait/throughput << "\t\t | \t\t" << totTurnAround/throughput << "\t\t | \t" << endl;
	
	// writing statistics to file //
	
	int filefd = open("processes_stats.txt", O_WRONLY);
	dup2(filefd, 1);
	
	cout << "Proc Name\t | \tArrival Time\t | \tBurst Time\t | \tTurnaround Time\t | \tWaiting Time\t |" << endl;
	
	for (int i = 0; i < procList.size(); i++) {
		tempProc = procList[i];
		
		cout << tempProc.name << "\t\t | \t" << tempProc.artime << "\t\t | \t" << tempProc.burst << "\t\t | \t" << tempProc.cTime - tempProc.sTime << "\t\t | \t" << tempProc.waitingTime << "\t\t |" << endl;
	}
	
	cout << endl;
	cout << "Throughput\t | \tAverage Waiting Time\t | \tAverage Turnaround\t |" << endl;
	cout << throughput << "\t\t | \t" << totWait/throughput << "\t\t | \t\t" << totTurnAround/throughput << "\t\t | \t" << endl;
	
	close(filefd);
}
